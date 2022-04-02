#include "CellsEvo/core/logic/non_hunter_logic.h"

namespace cells_evo::logic {
void NonHunterLogic::MoveCells(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, core::Food> &food,
    unsigned int world_tick
) {
  for (auto&[_, cell] : cells) {
    auto direction = ChooseDirection(cell, food);
    Move(cell, direction, cell.speed_);
  }
}

core::Vector2<float> NonHunterLogic::ChooseDirection(
    core::Cell &cell,
    std::unordered_map<unsigned int, core::Food> &food
) {
  // todo probably it should be generic, use for all types of cell
  core::Vector2<float> direction{};
  auto closest_food = FindClosestFood(cell, food);
  if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
    direction = GetRandomDirection(cell);
  } else {
    auto target_position = closest_food.value().GetPosition();
    direction = core::GetDirectionVector(cell.GetPosition(), target_position);
  }
  return direction;
}

std::optional<core::Food> NonHunterLogic::FindClosestFood(core::Cell &cell, std::unordered_map<unsigned int, core::Food> &foods) {
  if (foods.empty())
    return {};
  auto cached_closest_food = cell_food_cache_.find(cell.id_);
  if (cached_closest_food != cell_food_cache_.end()) {
    auto food_id = cached_closest_food->second;
    auto result = foods.find(food_id);
    if (result != foods.end())
      return result->second;
    else
      cell_food_cache_.erase(food_id);
  }

  unsigned int closest_food_idx;
  float min_distance = std::numeric_limits<float>::max();
  for (auto&[idx, food] : foods) {
    // todo seems position makes life harder
    auto dist = (food.GetPosition() - cell.GetPosition()).Magnitude();
    if (dist < min_distance) {
      min_distance = dist;
      closest_food_idx = idx;
    }
  }

  cell_food_cache_.insert({cell.id_, closest_food_idx});
  return foods.find(closest_food_idx)->second;
}

void NonHunterLogic::RebuildCellsFoodCache(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, core::Food> &food
) {
  cell_food_cache_.clear();
  for (auto&[_, cell] : cells) {
    FindClosestFood(cell, food);
  }
}

void NonHunterLogic::ProcessEatFood(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, core::Food> &food
) {
  for (auto&[_, cell] : cells) {
    auto closest_food = FindClosestFood(cell, food);
    if (closest_food && CellGotFood(cell, closest_food.value())) {
      cell.AddEnergy(core::Food::k_default_energy_value_);
      food.erase(closest_food.value().id_);
    }
  }
}

bool NonHunterLogic::CellGotFood(core::Cell &cell, core::Food &food) {
  return (cell.GetPosition() - food.GetPosition()).Magnitude() < cell.size_;
}

core::Vector2<float> NonHunterLogic::GetRandomDirection(core::Cell &cell) {
  auto cached_direction = cell_direction_cache_.find(cell.id_);
  auto cached_wait = cell_wait_cache_.find(cell.id_);
  // todo clean me when a cell is dead
  if (cached_wait == cell_wait_cache_.end()) {
    cell_wait_cache_.insert({cell.id_, 0});
    cached_wait = cell_wait_cache_.find(cell.id_);
  }
  cached_wait->second += 1;
  if (
      cached_direction == cell_direction_cache_.end() || cached_wait->second % cell.GetDirectionChangeFactor() == 0
  ) {
    cell_direction_cache_.erase(cell.id_);
    auto coords = core::GetRandomFloats(-1, 1, 2);
    auto direction = core::Vector2<float>(coords[0], coords[1]);
    direction.Normalize();
    cell_direction_cache_.insert({cell.id_, direction});
    return direction;
  }
  return cached_direction->second;
}
}
