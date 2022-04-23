#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_

#include "CellsEvo/cell.h"
#include "CellsEvo/genetics/engineer.h"
#include "cell_logic.h"
#include "CellsEvo/world.h"
#include "CellsEvo/collision_resolution.h"

namespace cells_evo::logic {
class Logic {
  const uint k_deleted_clear_threshold_ = 200;
  uint deleted_cells_num_ = 0;
  uint deleted_food_num_ = 0;
  cells_evo::core::World &world_;
  HunterCellLogic hunter_cell_logic_{};
  NonHunterCellLogic non_hunter_cell_logic_{};
  // todo maybe only cell generator should own genetics engineer?
  genetics::Engineer genetic_engineer_{};
  core::RandomEngine random_generator_{};
  float food_production_rate_;

  void Tick();
  void GenerateFood();
  void UpdateCellsState();
  [[maybe_unused]] void TeleportCrossedBoundaries(core::Cell &cell) const;
  void CheckCrossedBoundaries(core::Cell &cell) const;
  void MoveCells();
  void ChooseDirections();
  bool ShouldGenerateFood();
  bool CanEat(collisions::IdxPair &cell_id_pair);
  void HunterEat(collisions::IdXPairs &colliding_cell_ids);
  void NonHunterEat(collisions::FoodCellCollisions &colliding_food_cell_ids);
  core::Cell DivideCell(core::Cell &cell);
  core::Cell &ExtractHunter(collisions::IdxPair &cell_id_pair);
  core::Cell &ExtractPrey(collisions::IdxPair &cell_id_pair);
  void ResolveCellCollisions(collisions::IdXPairs &colliding_cells_ids) const;
  [[nodiscard]] bool ShouldCleanCells() const;
  [[nodiscard]] bool ShouldCleanFood() const;
  void CleanFood();
  void CleanCells();
  static bool ShouldChangeDirection(core::Cell &cell);

 public:
  explicit Logic(core::World &world, float food_production_rate);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_LOGIC_H_
