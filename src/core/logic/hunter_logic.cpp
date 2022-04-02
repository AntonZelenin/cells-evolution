#include "CellsEvo/core/logic/hunter_logic.h"

namespace cells_evo::logic {
template<typename T>
std::optional<T> CellLogic::FindClosestFood(
    core::Cell &cell,
    std::unordered_map<unsigned int, T> &food_entities
) {
  if (food_entities.empty())
    return {};
  auto cached_closest_food = cell_food_cache_.find(cell.id_);
  if (cached_closest_food != cell_food_cache_.end()) {
    auto food = food_entities.find(cached_closest_food->second);
    if (food != food_entities.end())
      return food->second;
    else
      cell_food_cache_.erase(cell.id_);
  }

  unsigned int closest_food_idx;
  float min_distance = std::numeric_limits<float>::max();
  for (auto&[idx, food] : food_entities) {
    auto dist = (cell.GetPosition() - food.GetPosition()).Magnitude();
    if (dist < min_distance) {
      min_distance = dist;
      closest_food_idx = idx;
    }
  }

  cell_food_cache_.insert({cell.id_, closest_food_idx});
  return food_entities.find(closest_food_idx)->second;
}

template<typename T>
void CellLogic::MoveCells(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, T> &food_entities,
    unsigned int world_tick
) {
  // update once in 10 frames
  if (world_tick % 10 == 0) cell_food_cache_.clear();
  for (auto&[_, cell] : cells) {
    auto direction = ChooseDirection(cell, food_entities);
    Move(cell, direction, cell.speed_);
  }
}

template<typename T>
core::Vector2<float> CellLogic::ChooseDirection(
    core::Cell &cell,
    std::unordered_map<unsigned int, T> &food_entities
) {
  // todo probably it should be generic, use for all types of cell
  core::Vector2<float> direction{};
  auto closest_food = FindClosestFood(cell, food_entities);
  if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
    direction = GetRandomDirection(cell);
  } else {
    auto target_position = closest_food.value().GetPosition();
    direction = core::GetDirectionVector(cell.GetPosition(), target_position);
  }
  return direction;
}

bool CellLogic::CellGotFood(core::Cell &cell, core::Entity &food_entity) {
  return (cell.GetPosition() - food_entity.GetPosition()).Magnitude() < (cell.size_ + food_entity.GetSize());
}

core::Vector2<float> CellLogic::GetRandomDirection(core::Cell &cell) {
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

void CellLogic::RebuildCellsFoodCache(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, core::Food> &food
) {
  cell_food_cache_.clear();
  for (auto&[_, cell] : cells) {
    FindClosestFood(cell, food);
  }
}

template<typename T>
void CellLogic::ProcessEatFood(
    std::unordered_map<unsigned int, core::Cell> &cells,
    std::unordered_map<unsigned int, T> &food_entities
) {
  for (auto&[_, hunter_cell] : cells) {
    auto cell = FindClosestFood(hunter_cell, food_entities);
    if (cell && CellGotFood(hunter_cell, cell.value())) {
      hunter_cell.AddEnergy(T::k_default_energy_value_);
      // todo this is not good
      food_entities.erase(cell.value().id_);
    }
  }

}
}
