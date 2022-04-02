#include "CellsEvo/core/logic/hunter_logic.h"

namespace cells_evo::logic {
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
}
