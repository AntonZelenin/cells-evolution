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
  std::optional<core::Cell> FindClosestCell(core::Cell &hunter_cell, std::map<unsigned int, core::Cell> &cells);
 public:
  void MoveCells(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells,
      unsigned int world_tick
  );

  void ProcessEatCell(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells
  );

  // todo not duplicate?
  static bool HunterCellGotCell(core::Cell &hunter_cell, core::Cell &cell);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_
