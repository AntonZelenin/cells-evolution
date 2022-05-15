#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/cell.h"
#include "CellsEvo/world.h"

namespace cells_evo::logic {
class NonHunterCellLogic {
  core::RandomEngine random_engine_{};

  static std::optional<uint> FindClosestFoodIdx(
      core::Cell &cell,
      uint food_idx,
      core::FoodStorage &foods
  );
  static bool IsEatable(const core::Food &food);
  static bool IsDangerous(const core::Cell &cell);

 public:
  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity);
  void ChooseDirection(core::Cell &cell, uint& food_idx, uint cell_idx, core::FoodStorage &food, core::CellStorage &cells);
  core::Vector2<float> GetRandomDirection(core::Cell &cell);
  static std::optional<uint> FindClosestCellIdx(core::Cell &cell, uint cell_idx, core::CellStorage &cells);
};

class HunterCellLogic : public NonHunterCellLogic {
  static std::optional<uint> FindClosestCellIdx(
      core::Cell &cell,
      uint cell_idx,
      core::CellStorage &cells
  );
 public:
  void ChooseDirection(core::Cell &cell, uint cell_idx, core::CellStorage &cells);
  static bool IsEatable(const core::Cell &prey);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
