#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/genetics/engineer.h"
#include "cell_logic.h"
#include "CellsEvo/core/world.h"
#include "CellsEvo/core/collision_resolution.h"

namespace cells_evo::logic {
class Logic {
  cells_evo::core::World &world_;
  unsigned int food_production_rate_reverse_;

  HunterCellLogic hunter_cell_logic_{};
  NonHunterCellLogic non_hunter_cell_logic_{};
  genetics::Engineer genetic_engineer_{};
//  collisions::CollisionDetector collision_detector_{};
//  collisions::CollisionResolver collision_resolver_{};

  void CountTick();
  collisions::CellPtrPairs Eat(collisions::CellPtrPairs &colliding_cells);
  void GenerateFood();
  void DivideCells();
  std::shared_ptr<core::Cell> DivideCell(core::Cell &cell);
  void CheckCellsEnergy();
  void TeleportCrossedBoundaries();
  void CheckCrossedBoundaries();
  void MoveCells();
  void MoveCell(std::shared_ptr<core::Cell> &cell);
  static bool HunterGotPrey(collisions::CellPtrPair &cell_pair);
  static std::shared_ptr<core::Cell> &ExtractHunter(collisions::CellPtrPair &cell_pair);
  static std::shared_ptr<core::Cell> &ExtractPrey(collisions::CellPtrPair &cell_pair);

 public:
  explicit Logic(core::World &world, unsigned int food_production_rate);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_LOGIC_H_
