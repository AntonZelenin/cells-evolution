#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> const &direction);

class NonHunterCellLogic {
  virtual std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      core::EdibleEntityStorage &foods
  );

 public:
  virtual void MoveCell(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  void ProcessEatFood(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  // todo not duplicate?
  static bool CellGotFood(core::Cell &cell, core::Entity &food_entity);
  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity);
  core::Vector2<float> ChooseDirection(core::Cell &cell, core::EdibleEntityStorage &food_entities);
  static core::Vector2<float> GetRandomDirection(core::Cell &cell);
  void ChangeDirection(core::Cell &cell, core::EdibleEntityStorage &food_entities);
};

class HunterCellLogic : public NonHunterCellLogic {
  std::shared_ptr<core::EdibleEntity> FindClosestFood(
      core::Cell &cell,
      core::EdibleEntityStorage &cells
  ) override;
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
