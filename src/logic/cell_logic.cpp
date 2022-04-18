#include "CellsEvo/logic/cell_logic.h"

namespace cells_evo::logic {
bool NonHunterCellLogic::CellGotFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).MagnitudeSquared() <= pow((cell.GetSize() + food_entity.GetSize()), 2);
}

core::Vector2<float> NonHunterCellLogic::GetRandomDirection(core::Cell &cell) {
  auto coords = random_engine_.GetRandomFloats(-1, 1, 2);
  auto direction = core::Vector2<float>(coords[0], coords[1]);
  direction.Normalize();
  cell.SetDirection(direction);
  return direction;
}

void NonHunterCellLogic::MoveCell(core::Cell &cell, core::EdibleEntityStorage &food_entities) {
  // check for new food every N frames
  if (!cell.GetDirection() || cell.lifetime_ % 15 == 0) {
    cell.ClearFoodTarget();
    cell.SetDirection(ChooseDirection(cell, food_entities));
  } else if (cell.lifetime_ % cell.GetDirectionChangeFactor() == 0) {
    cell.SetDirection(GetRandomDirection(cell));
  }
  cell.Move(cell.GetDirection().value());
}

void NonHunterCellLogic::ProcessEatFood(core::Cell &cell, core::EdibleEntityStorage &food_entities) {
  auto food = FindClosestFood(cell, food_entities);
  if (food != nullptr && cell.IsHungry() && CellGotFood(cell, *food)) {
    cell.AddEnergy(food->GetNutritionValue());
    cell.ClearFoodTarget();
    food_entities.erase(food->GetId());
  }
}

bool NonHunterCellLogic::CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).MagnitudeSquared() <= pow(cell.GetMaxFoodDetectionDistance(), 2);
}

core::Vector2<float> NonHunterCellLogic::ChooseDirection(core::Cell &cell, core::EdibleEntityStorage &food_entities) {
  std::optional<core::Vector2<float>> direction = cell.GetDirection();
  auto closest_food = FindClosestFood(cell, food_entities);
  if (!closest_food && !direction) {
    direction = GetRandomDirection(cell);
  } else if (closest_food) {
    direction = core::GetDirectionVector(cell.GetPosition(), closest_food->GetPosition());
  }
  return direction.value();
}

std::shared_ptr<core::EdibleEntity> NonHunterCellLogic::FindClosestFood(
    core::Cell &cell,
    core::EdibleEntityStorage &foods
) {
  if (foods.empty())
    return nullptr;
  auto cached_closest_food_id = cell.GetFoodTargetId();
  if (cached_closest_food_id) {
    auto food = foods.find(cached_closest_food_id.value());
    if (food != foods.end())
      return food->second;
  }

  uint closest_food_idx;
  float min_distance_sqared = std::numeric_limits<float>::max();
  for (auto&[idx, food] : foods) {
    auto dist_squared = (cell.GetPosition() - food->GetPosition()).MagnitudeSquared();
    if (dist_squared < min_distance_sqared) {
      min_distance_sqared = dist_squared;
      closest_food_idx = idx;
    }
  }
  if (!CouldSensedFood(cell, *foods.at(closest_food_idx)))
    return {};

  cell.SetFoodTargetId(closest_food_idx);
  return foods.at(closest_food_idx);
}

std::shared_ptr<core::EdibleEntity> HunterCellLogic::FindClosestFood(
    core::Cell &cell,
    core::EdibleEntityStorage &cells
) {
  if (cells.empty())
    return {};
  auto cached_closest_food = cell.GetFoodTargetId();
  if (cached_closest_food) {
    auto food = cells.find(cached_closest_food.value());
    if (food != cells.end())
      return food->second;
  }

  uint closest_food_idx = 0;
  float min_distance_squared = std::numeric_limits<float>::max();
  for (auto&[idx, prey_cell] : reinterpret_cast<core::CellStorage &>(cells)) {
    if (prey_cell->IsHunter() && !prey_cell->IsDead()) continue;
    auto dist_squared = (cell.GetPosition() - prey_cell->GetPosition()).MagnitudeSquared();
    if (dist_squared < min_distance_squared) {
      min_distance_squared = dist_squared;
      closest_food_idx = idx;
    }
  }
  if (closest_food_idx == 0) return {};

  if (!CouldSensedFood(cell, *cells.at(closest_food_idx)))
    return {};

  cell.SetFoodTargetId(closest_food_idx);
  return cells.at(closest_food_idx);
}
}
