#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/cell.h"
#include "CellsEvo/world.h"

namespace cells_evo::logic {
class NonHunterCellLogic {
  core::RandomEngine random_engine_{};

  static std::optional<uint> FindClosestFoodId(
      core::Cell &cell,
      // todo cell depends on world, is it ok? Where to put type definitions?
      core::FoodStorage &foods
  );

 public:
  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity);
  void ChooseDirection(core::Cell &cell, core::FoodStorage &food);
  core::Vector2<float> GetRandomDirection(core::Cell &cell);
};

class HunterCellLogic : public NonHunterCellLogic {
  static std::optional<uint> FindClosestCellId(
      core::Cell &cell,
      uint cell_idx,
      core::CellStorage &cells
  );
 public:
  void ChooseDirection(core::Cell &cell, uint cell_idx,  core::CellStorage &cells);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
