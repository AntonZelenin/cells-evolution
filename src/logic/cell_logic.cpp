#include "CellsEvo/logic/cell_logic.h"

namespace cells_evo::logic {
bool NonHunterCellLogic::CellGotFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).MagnitudeSquared()
      <= pow((cell.GetSize() + food_entity.GetSize()), 2);
}

core::Vector2<float> NonHunterCellLogic::GetRandomDirection(core::Cell &cell) {
  auto coords = random_engine_.GetRandomFloats(-1, 1, 2);
  auto direction = core::Vector2<float>(coords[0], coords[1]);
  direction.Normalize();
  cell.SetDirection(direction);
  return direction;
}

void NonHunterCellLogic::MoveCell(core::Cell &cell,
                                  core::EdibleEntityStorage &food_entities,
                                  core::Field &tiled_field) {
  // check for new food every N frames
  if (!cell.GetDirection() || cell.lifetime_ % 15 == 0) {
    cell.ClearFoodTarget();
    cell.SetDirection(ChooseDirection(cell, food_entities, tiled_field));
  } else if (cell.lifetime_ % cell.GetDirectionChangeFactor() == 0) {
    cell.SetDirection(GetRandomDirection(cell));
  }
  cell.Move(cell.GetDirection().value());
}

void NonHunterCellLogic::ProcessEatFood(
    core::Cell &cell,
    core::EdibleEntityStorage &food_entities,
    core::Field &tiled_field
) {
  auto food = FindClosestFood(cell, food_entities, tiled_field);
  if (food != nullptr && cell.IsHungry() && CellGotFood(cell, *food)) {
    cell.AddEnergy(food->GetNutritionValue());
    cell.ClearFoodTarget();
    food_entities.erase(food->GetId());
  }
}

bool NonHunterCellLogic::CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).MagnitudeSquared()
      <= pow(cell.GetMaxFoodDetectionDistance(), 2);
}

core::Vector2<float> NonHunterCellLogic::ChooseDirection(core::Cell &cell,
                                                         core::EdibleEntityStorage &food_entities,
                                                         core::Field &tiled_field) {
  std::optional<core::Vector2<float>> direction = cell.GetDirection();
  auto closest_food = FindClosestFood(cell, food_entities, tiled_field);
  if (!closest_food && !direction) {
    direction = GetRandomDirection(cell);
  } else if (closest_food) {
    direction = core::GetDirectionVector(cell.GetPosition(), closest_food->GetPosition());
  }
  return direction.value();
}

std::shared_ptr<core::EdibleEntity> NonHunterCellLogic::FindClosestFood(
    core::Cell &cell,
    core::EdibleEntityStorage &foods,
    core::Field &tiled_field
) {
  if (foods.empty())
    return nullptr;
  auto cached_closest_food_id = cell.GetFoodTargetId();
  if (cached_closest_food_id) {
    auto food = foods.find(cached_closest_food_id.value());
    if (food != foods.end())
      return food->second;
  }

  uint closest_food_id = 0;
  float min_distance_squared = std::numeric_limits<float>::max();

  auto tiles = tiled_field.GetNeighbouringTileIndices(&cell);
  for (auto &tile_id : tiles) {
    for (auto &entity : tiled_field.tiles_[tile_id]) {
      // todo
      if (entity.expired()) continue;
      auto ent = entity.lock();
      if (ent->GetEntityType() == core::EntityType::K_FOOD) {
        auto dist_squared = (cell.GetPosition() - ent->GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_id = ent->GetId();
        }
      }
    }
  }
  if (closest_food_id == 0) return {};

//  for (auto &[id, food] : foods) {
//    auto dist_squared = (cell.GetPosition() - food->GetPosition()).MagnitudeSquared();
//    if (dist_squared < min_distance_squared) {
//      min_distance_squared = dist_squared;
//      closest_food_id = id;
//    }
//  }
  if (!CouldSensedFood(cell, *foods.at(closest_food_id)))
    return {};

  cell.SetFoodTargetId(closest_food_id);
  return foods.at(closest_food_id);
}

std::shared_ptr<core::EdibleEntity> HunterCellLogic::FindClosestFood(
    core::Cell &cell,
    core::EdibleEntityStorage &cells,
    core::Field &tiled_field
) {
  if (cells.empty())
    return {};
  auto cached_closest_food = cell.GetFoodTargetId();
  if (cached_closest_food) {
    auto food = cells.find(cached_closest_food.value());
    if (food != cells.end())
      return food->second;
  }

  uint closest_food_id = 0;
  float min_distance_squared = std::numeric_limits<float>::max();

  auto tiles = tiled_field.GetNeighbouringTileIndices(&cell);
  for (auto &tile_id : tiles) {
    for (auto &entity : tiled_field.tiles_[tile_id]) {
      // todo
      if (entity.expired()) continue;
      // todo you can improve closest neighbour search because the array is sorted
      auto ent = std::static_pointer_cast<core::Cell>(entity.lock());
      if (ent->GetEntityType() == core::EntityType::K_CELL) {
        if (ent->IsHunter() && !ent->IsDead()) continue;
        auto dist_squared = (cell.GetPosition() - ent->GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_id = ent->GetId();
        }
      }
    }
  }
  if (closest_food_id == 0) return {};

//  uint closest_food_id = 0;
//  float min_distance_squared = std::numeric_limits<float>::max();
//  for (auto &[idx, prey_cell] : reinterpret_cast<core::CellStorage &>(cells)) {
//    if (prey_cell->IsHunter() && !prey_cell->IsDead()) continue;
//    auto dist_squared = (cell.GetPosition() - prey_cell->GetPosition()).MagnitudeSquared();
//    if (dist_squared < min_distance_squared) {
//      min_distance_squared = dist_squared;
//      closest_food_id = idx;
//    }
//  }
//  if (closest_food_id == 0) return {};

  if (!CouldSensedFood(cell, *cells.at(closest_food_id)))
    return {};

  cell.SetFoodTargetId(closest_food_id);
  return cells.at(closest_food_id);
}
}
