#include "CellsEvo/logic/cell_logic.h"

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
void NonHunterCellLogic::ChooseDirection(core::Cell &cell, uint& food_idx, core::FoodStorage &food) {
  core::Vector2<float> direction;
  auto closest_food_idx = FindClosestFoodIdx(cell, food_idx, food);
  if (!closest_food_idx.has_value()) {
    direction = GetRandomDirection(cell);
    cell.ClearFoodTarget();
  } else {
    direction = core::GetDirectionVector(cell.GetPosition(), food.at(closest_food_idx.value()).GetPosition());
    cell.SetHasFoodTarget();
    food_idx = closest_food_idx.value();
  }
  cell.SetDirection(direction);
}

void HunterCellLogic::ChooseDirection(core::Cell &cell, uint cell_idx, core::CellStorage &cells) {
  core::Vector2<float> direction;
  auto closest_prey_id = FindClosestCellIdx(cell, cell_idx, cells);
  if (!closest_prey_id.has_value()) {
    direction = GetRandomDirection(cell);
    cell.ClearFoodTarget();
  } else {
    direction = core::GetDirectionVector(cell.GetPosition(), cells[closest_prey_id.value()].GetPosition());
    cell.SetHasFoodTarget();
  }
  cell.SetDirection(direction);
}

std::optional<uint> HunterCellLogic::FindClosestCellIdx(
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
        if (dist_squared < min_distance_squared && CouldSensedFood(cell, cells.at(closest_x_prey_idx_left))) {
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
    while (closest_x_prey_idx_right <= cells.size()) {
      auto &prey = cells.at(closest_x_prey_idx_right);

      if (IsEatable(prey)) {
        if (pow(cell.GetPosition().x - prey.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - prey.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared && CouldSensedFood(cell, cells.at(closest_x_prey_idx_right))) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_prey_idx_right;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_prey_idx_right == cells.size() - 1) break;
      ++closest_x_prey_idx_right;
    }
  }

  return closest_food_idx;
}

// todo not exactly duplicate
std::optional<uint> NonHunterCellLogic::FindClosestFoodIdx(
    core::Cell &cell,
    uint food_idx,
    core::FoodStorage &foods
) {
  // todo sometimes cells choose not the closest food
  // todo sometimes cells can't eat food
  if (foods.empty()) return {};
  std::optional<uint> closest_food_idx = {};

  // todo when multiple hunters eat one cell program crashes
  // todo hunters sometimes choose not the closest cell
  float min_distance_squared = std::numeric_limits<float>::max();
  // todo if and while can be replaced with for
  auto closest_x_food_idx_left = food_idx;
  while (closest_x_food_idx_left >= 0) {
    auto &food = foods.at(closest_x_food_idx_left);

    if (IsEatable(food)) {
      if (pow(cell.GetPosition().x - food.GetPosition().x, 2) >= min_distance_squared) break;
      auto dist_squared = (cell.GetPosition() - food.GetPosition()).MagnitudeSquared();
      if (dist_squared < min_distance_squared && CouldSensedFood(cell, foods.at(closest_x_food_idx_left))) {
        min_distance_squared = dist_squared;
        closest_food_idx = closest_x_food_idx_left;
      }
    }

    // so that there's no overflow of uint
    if (closest_x_food_idx_left == 0) break;
    --closest_x_food_idx_left;
  }

  if (food_idx < foods.size() - 1) {
    auto closest_x_food_idx_right = food_idx + 1;
    while (closest_x_food_idx_right <= foods.size() - 1) {
      auto &food = foods.at(closest_x_food_idx_right);

      if (IsEatable(food)) {
        if (pow(cell.GetPosition().x - food.GetPosition().x, 2) > min_distance_squared) break;
        auto dist_squared = (cell.GetPosition() - food.GetPosition()).MagnitudeSquared();
        if (dist_squared < min_distance_squared && CouldSensedFood(cell, foods.at(closest_x_food_idx_right))) {
          min_distance_squared = dist_squared;
          closest_food_idx = closest_x_food_idx_right;
        }
      }

      // so that there's no overflow of uint
      if (closest_x_food_idx_right == foods.size() - 1) break;
      ++closest_x_food_idx_right;
    }
  }

  return closest_food_idx;
}

bool HunterCellLogic::IsEatable(const core::Cell &prey) {
  return !prey.IsDeleted() && (!prey.IsHunter() || prey.IsDead());
}

bool NonHunterCellLogic::IsEatable(const core::Food &food) {
  return !food.IsDeleted();
}
}
