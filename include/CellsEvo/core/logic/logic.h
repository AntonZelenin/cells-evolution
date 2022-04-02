#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/genetics/engineer.h"
#include "hunter_logic.h"
#include "non_hunter_logic.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {

// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class Logic {
  cells_evo::core::World &world_;
  unsigned int world_ticks_ = 0;
  unsigned int food_production_rate_reverse_;

  HunterLogic hunter_logic_{};
  NonHunterLogic non_hunter_logic_{};
  genetics::Engineer genetic_engineer_{};

  void CountTick();
  void Eat();
  void GenerateFood();
  void DivideCells();
  core::Cell DivideCell(core::Cell &cell);
  void CheckCellsEnergy();

 public:
  explicit Logic(core::World &world, unsigned int food_production_rate);

  void WorldTick();
  void MoveCells();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_
