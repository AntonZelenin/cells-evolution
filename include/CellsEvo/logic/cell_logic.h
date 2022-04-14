#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/cell.h"
#include "CellsEvo/world.h"

namespace cells_evo::logic {
class NonHunterCellLogic {
  core::RandomEngine random_engine_{};

  virtual std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      // todo cell depends on world, is it ok? Where to put type definitions?
      core::EdibleEntityStorage &foods
  );

 public:
  virtual void MoveCell(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  void ProcessEatFood(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  static bool CellGotFood(core::Cell &cell, core::Entity &food_entity);
  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity);
  core::Vector2<float> ChooseDirection(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  core::Vector2<float> GetRandomDirection(core::Cell &cell);
};

class HunterCellLogic : public NonHunterCellLogic {
  std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      core::EdibleEntityStorage &cells
  ) override;
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_LOGIC_CELL_LOGIC_H_
