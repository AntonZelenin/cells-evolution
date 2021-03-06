#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
class NonHunterCellLogic {
  core::RandomEngine random_engine_{};

  virtual bool IsEatable(const core::Food &food);
  static bool IsDangerous(const core::Cell &cell);

 protected:
  std::optional<uint> FindClosestFoodIdx(
      core::Cell &cell,
      uint food_idx,
      core::FoodStorage &foods
  );

 public:
  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity);
  void ChooseDirection(core::Cell &cell, uint& food_idx, uint cell_idx, core::FoodStorage &food, core::CellStorage &cells);
  core::Vector2<float> GetRandomDirection(core::Cell &cell);
  static std::optional<uint> FindClosestCellIdx(core::Cell &cell, uint cell_idx, core::CellStorage &cells);
  static void HeadTo(core::Cell &cell, core::Position &position);
};

class HunterCellLogic : public NonHunterCellLogic {
  static std::optional<uint> FindClosestCellIdx(
      core::Cell &cell,
      uint cell_idx,
      core::CellStorage &cells
  );
 public:
  void ChooseDirection(core::Cell &cell, uint cell_idx, core::CellStorage &cells, uint food_idx, core::FoodStorage &food);
  static bool IsEatable(const core::Cell &prey);
  bool IsEatable(const core::Food &food) override;
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
