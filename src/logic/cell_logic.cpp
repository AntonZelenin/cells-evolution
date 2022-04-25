#include "CellsEvo/logic/cell_logic.h"
#include "CellsEvo/collision_resolution.h"

namespace cells_evo::logic {
core::Vector2<float> NonHunterCellLogic::GetRandomDirection(core::Cell &cell) {
  auto coords = random_engine_.GetRandomFloats(-1, 1, 2);
  auto direction = core::Vector2<float>(coords[0], coords[1]);
  direction.Normalize();
  cell.SetDirection(direction);
  return direction;
}

bool NonHunterCellLogic::CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).MagnitudeSquared()
      <= pow(cell.GetMaxFoodDetectionDistance(), 2);
}

// todo duplicate
void NonHunterCellLogic::ChooseDirection(core::Cell &cell, core::FoodStorage &food) {
  core::Vector2<float> direction;
  auto closest_food_id = FindClosestFoodId(cell, food);
  if (!closest_food_id.has_value()) {
    direction = GetRandomDirection(cell);
    cell.ClearFoodTarget();
  } else {
    direction = core::GetDirectionVector(cell.GetPosition(), food.at(closest_food_id.value()).GetPosition());
    cell.SetHasFoodTarget();
  }
  cell.SetDirection(direction);
}

void HunterCellLogic::ChooseDirection(core::Cell &cell, uint cell_idx, core::CellStorage &cells) {
  core::Vector2<float> direction;
  auto closest_prey_id = FindClosestCellId(cell, cell_idx, cells);
  if (!closest_prey_id.has_value()) {
    direction = GetRandomDirection(cell);
    cell.ClearFoodTarget();
  } else {
    direction = core::GetDirectionVector(cell.GetPosition(), cells[closest_prey_id.value()].GetPosition());
    cell.SetHasFoodTarget();
  }
  cell.SetDirection(direction);
}

std::optional<uint> NonHunterCellLogic::FindClosestFoodId(
    core::Cell &cell,
    core::FoodStorage &foods
) {
  if (foods.empty())
    return {};

  // todo
  auto closest_food_idx = collisions::CollisionDetector::FindClosestXFoodIdx(cell, foods);

  if (!closest_food_idx.has_value()) return {};

  float min_distance_squared = std::numeric_limits<float>::max();
  if (closest_food_idx >= 0) {
    auto closest_x_food_idx_left = closest_food_idx.value();
    while (closest_x_food_idx_left >= 0) {
      auto &food = foods.at(closest_x_food_idx_left);

      if (!food.IsDeleted()) {
        if (pow(cell.GetPosition().x - food.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - food.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_food_idx_left;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_food_idx_left == 0) break;
      --closest_x_food_idx_left;
    }
  }

  if (closest_food_idx < foods.size() - 1) {
    auto closest_x_food_idx_right = closest_food_idx.value() + 1;
    while (closest_x_food_idx_right < foods.size()) {
      auto &food = foods.at(closest_x_food_idx_right);

      if (!food.IsDeleted()) {
        if (pow(cell.GetPosition().x - food.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - food.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_food_idx_right;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_food_idx_right == foods.size() - 1) break;
      ++closest_x_food_idx_right;
    }
  }

  if (!CouldSensedFood(cell, foods.at(closest_food_idx.value())))
    return {};

  return closest_food_idx;
}

std::optional<uint> HunterCellLogic::FindClosestCellId(
    core::Cell &cell,
    uint cell_idx,
    core::CellStorage &cells
) {
  if (cells.size() < 2) return {};
  std::optional<uint> closest_food_idx = {};

  // todo when multiple hunters eat one cell program crashes
  // todo hunters sometimes choose not the closest cell
  float min_distance_squared = std::numeric_limits<float>::max();
  if (cell_idx > 0) {
    auto closest_x_prey_idx_left = cell_idx - 1;
    while (closest_x_prey_idx_left >= 0) {
      auto &prey = cells.at(closest_x_prey_idx_left);

      if (IsEatable(prey)) {
        if (pow(cell.GetPosition().x - prey.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - prey.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_prey_idx_left;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_prey_idx_left == 0) break;
      --closest_x_prey_idx_left;
    }
  }

  if (cell_idx < cells.size() - 1) {
    auto closest_x_prey_idx_right = cell_idx + 1;
    while (closest_x_prey_idx_right < cells.size()) {
      auto &prey = cells.at(closest_x_prey_idx_right);

      if (IsEatable(prey)) {
        if (pow(cell.GetPosition().x - prey.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - prey.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_prey_idx_right;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_prey_idx_right == cells.size() - 1) break;
      ++closest_x_prey_idx_right;
    }
  }

  if (!closest_food_idx.has_value() || !CouldSensedFood(cell, cells.at(closest_food_idx.value())))
    return {};

  return closest_food_idx;
}

bool HunterCellLogic::IsEatable(const core::Cell &prey) {
  return !prey.IsDeleted() && (!prey.IsHunter() || prey.IsDead());
}
}
