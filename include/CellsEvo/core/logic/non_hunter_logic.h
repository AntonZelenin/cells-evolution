#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {

// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class NonHunterLogic {
  std::map<unsigned int, unsigned int> cell_food_cache_;
 public:
  void MoveCells(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food,
      unsigned int world_tick
  );

  core::Vector2<float> ChooseDirection(core::Cell &cell, std::map<unsigned int, core::Food> &food);

  // todo I want it to return a reference, do I?
  std::optional<core::Food> FindClosestFood(core::Cell &cell, std::map<unsigned int, core::Food> &foods);

  static bool CouldSensedFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  core::Vector2<float> GetRandomSinDirection();

  void RebuildCellsFoodCache(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  );

  void ProcessEatFood(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  );

  static bool CellGotFood(core::Cell &cell, core::Food &food);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
