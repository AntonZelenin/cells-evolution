#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/food.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {

// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class NonHunterLogic {
 public:
  void MoveCells(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, core::Food> &food,
      unsigned int world_tick
  );

  void RebuildCellsFoodCache(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, core::Food> &food
  );

  void ProcessEatFood(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, core::Food> &food
  );

 private:
  std::unordered_map<unsigned int, unsigned int> cell_food_cache_;
  std::unordered_map<unsigned int, core::Vector2<float>> cell_direction_cache_;
  std::unordered_map<unsigned int, int> cell_wait_cache_;

  core::Vector2<float> ChooseDirection(
      core::Cell &cell,
      std::unordered_map<unsigned int, core::Food> &food
  );

  // todo I want it to return a reference, do I?
  std::optional<core::Food> FindClosestFood(core::Cell &cell, std::unordered_map<unsigned int, core::Food> &foods);

  static bool CouldSensedFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  core::Vector2<float> GetRandomDirection(core::Cell &cell);

  static bool CellGotFood(core::Cell &cell, core::Food &food);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_NON_HUNTER_LOGIC_H_
