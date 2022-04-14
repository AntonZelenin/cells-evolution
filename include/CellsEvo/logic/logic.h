#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_

#include "CellsEvo/cell.h"
#include "CellsEvo/genetics/engineer.h"
#include "cell_logic.h"
#include "CellsEvo/world.h"
#include "CellsEvo/collision_resolution.h"

namespace cells_evo::logic {
class Logic {
  cells_evo::core::World &world_;
  HunterCellLogic hunter_cell_logic_{};
  NonHunterCellLogic non_hunter_cell_logic_{};
  // todo maybe only cell generator should own genetics engineer?
  genetics::Engineer genetic_engineer_{};
  core::RandomEngine random_generator_{};
  float food_production_rate_;
//  collisions::CollisionDetector collision_detector_{};
//  collisions::CollisionResolver collision_resolver_{};

  void CountTick();
  void GenerateFood();
  void UpdateCellsState();
  void TeleportCrossedBoundaries();
  void CheckCrossedBoundaries();
  void MoveCells();
  void MoveCell(std::shared_ptr<core::Cell> &cell);
  bool ShouldGenerateFood();
  static bool CanEat(collisions::CellPtrPair &cell_pair);
  collisions::CellPtrPairs Eat(collisions::CellPtrPairs &colliding_cells);
  std::shared_ptr<core::Cell> DivideCell(core::Cell &cell);
  static std::shared_ptr<core::Cell> &ExtractHunter(collisions::CellPtrPair &cell_pair);
  static std::shared_ptr<core::Cell> &ExtractPrey(collisions::CellPtrPair &cell_pair);

 public:
  explicit Logic(core::World &world, float food_production_rate);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_
