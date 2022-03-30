#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"
#include "hunter_logic.h"
#include "non_hunter_logic.h"

namespace cells_evo::logic {

// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class Logic {
  core::World &world_;
  unsigned int world_ticks_ = 0;
  unsigned int food_production_rate_reverse_;

  HunterLogic hunter_logic_{};
  NonHunterLogic non_hunter_logic_{};

  void CountTick();

  void Eat();

  void GenerateFood();

  void DivideCells();

  static core::Cell DivideCell(core::Cell &cell);

  void CheckCellsEnergy();

 public:
  explicit Logic(core::World &world, unsigned int food_production_rate);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
