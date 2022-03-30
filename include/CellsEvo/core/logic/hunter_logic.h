#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class HunterLogic {
  std::map<unsigned int, unsigned int> hunter_cell_cell_cache_;

  // todo looks like duplicate
  std::optional<core::Cell> FindClosestCell(core::Cell &hunter_cell, std::map<unsigned int, core::Cell> &cells) {
    if (cells.empty())
      return {};
    auto cached_closest_cell = hunter_cell_cell_cache_.find(hunter_cell.id_);
    if (cached_closest_cell != hunter_cell_cell_cache_.end()) {
      auto hunter_cell_id = cached_closest_cell->first;
      auto cell_id = cached_closest_cell->second;
      auto cell = cells.find(cell_id);
      if (cell != cells.end())
        return cell->second;
      else
        hunter_cell_cell_cache_.erase(hunter_cell_id);
    }

    unsigned int closest_cell_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, cell] : cells) {
      auto dist = (cell.GetPosition() - hunter_cell.GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_cell_idx = idx;
      }
    }

    hunter_cell_cell_cache_.insert({hunter_cell.id_, closest_cell_idx});
    return cells.find(closest_cell_idx)->second;
  }
 public:
  void MoveCells(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells,
      unsigned int world_tick
  ) {
    // update once in 10 frames
    if (world_tick % 10 == 0) hunter_cell_cell_cache_.clear();
    for (auto&[_, hunter_cell] : hunter_cells) {
      auto closest_cell = FindClosestCell(hunter_cell, cells);
      if (!closest_cell) return;
      auto target_position = closest_cell.value().GetPosition();
      auto direction = core::GetDirectionVector(hunter_cell.GetPosition(), target_position);
      Move(hunter_cell, direction, hunter_cell.speed_);
    }
  }

  void ProcessEatCell(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells
  ) {
    for (auto&[_, hunter_cell] : hunter_cells) {
      auto cell = FindClosestCell(hunter_cell, cells);
      if (cell && HunterCellGotCell(hunter_cell, cell.value())) {
        hunter_cell.AddEnergy(core::Cell::k_default_energy_value_);
        // todo this is not good
        cells.erase(cell.value().id_);
      }
    }
  }

  // todo not duplicate?
  static bool HunterCellGotCell(core::Cell &hunter_cell, core::Cell &cell) {
    return (hunter_cell.GetPosition() - cell.GetPosition()).Magnitude() < (hunter_cell.size_ + cell.size_);
  }
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_
