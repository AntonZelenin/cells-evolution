#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class CellLogic {
  std::unordered_map<unsigned int, unsigned int> cell_food_cache_;
  std::unordered_map<unsigned int, core::Vector2<float>> cell_direction_cache_;
  std::unordered_map<unsigned int, int> cell_wait_cache_;

  // todo looks like duplicate
  template<typename T>
  std::optional<T> FindClosestFood(
      core::Cell &cell,
      std::unordered_map<unsigned int, T> &food_entities
  );
 public:
  template<typename T>
  void MoveCells(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, T> &food_entities,
      unsigned int world_tick
  );

  template<typename T>
  void ProcessEatFood(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, T> &food_entities
  );

  // todo not duplicate?
  static bool CellGotFood(core::Cell &cell, core::Entity &food_entity);

  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
    return (cell.GetPosition() - food_entity.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  template<typename T>
  core::Vector2<float> ChooseDirection(core::Cell &cell, std::unordered_map<unsigned int, T> &food_entities);
  core::Vector2<float> GetRandomDirection(core::Cell &cell);
  void RebuildCellsFoodCache(
      std::unordered_map<unsigned int, core::Cell> &cells,
      std::unordered_map<unsigned int, core::Food> &food
  );
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_HUNTER_LOGIC_H_
