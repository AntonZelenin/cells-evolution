#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_

#include "cell.h"
#include "world.h"

namespace cells_evo::logic {
class Logic {
  core::World &world_;
  std::map<unsigned int, unsigned int> cell_food_cache_;

  std::optional<core::Food> FindClosestFood(core::Cell &cell);

  void MoveCells();

  void ProcessEvents();

  void RebuildCellsFoodCache();

  static bool CellGotFood(core::Cell &cell, core::Food &food);

  void RemoveFoodFromCache(const core::Food &food);

  void ProcessEatFood(core::Cell &cell);

  void GenerateFood();

  void DivideCells();

  void CheckCellsEnergy();

  void KillCell(unsigned int cell_id);

 public:
  explicit Logic(core::World &world);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
