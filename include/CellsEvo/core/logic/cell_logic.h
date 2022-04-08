#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_

#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/world.h"

namespace cells_evo::logic {
// todo remove, it's temporary
void Move(core::Cell &cell, core::Vector2<float> &direction, float speed);

class CellLogic {
  template<typename T>
  std::optional<T> FindClosestFood(
      core::Cell &cell,
      std::unordered_map<unsigned int, T> &food_entities
  ) {
    if (food_entities.empty())
      return {};
    auto cached_closest_food = cell.GetFoodTargetId();
    if (cached_closest_food) {
      auto food = food_entities.find(cached_closest_food.value());
      if (food != food_entities.end())
        return food->second;
    }

    unsigned int closest_food_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, food] : food_entities) {
      auto dist = (cell.GetPosition() - food.GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_food_idx = idx;
      }
    }

    cell.SetFoodTargetId(closest_food_idx);
    return food_entities.find(closest_food_idx)->second;
  }

 public:
  template<typename T>
  void MoveCells(
      core::Cell &cell,
      std::unordered_map<unsigned int, T> &food_entities
  ) {
    // check for new food every N frames
    if (cell.lifetime_ % 15 == 0) cell.ClearFoodTarget();
    auto direction = ChooseDirection(cell, food_entities);
    Move(cell, direction, cell.GetSpeed());
  }

  template<typename T>
  void ProcessEatFood(
      core::Cell &cell,
      std::unordered_map<unsigned int, T> &food_entities
  ) {
    auto food = FindClosestFood(cell, food_entities);
    if (food && CellGotFood(cell, food.value())) {
      cell.AddEnergy(food.value().GetNutritionValue());
      // todo this is not good
      food_entities.erase(food.value().id_);
    }
  }

  // todo not duplicate?
  static bool CellGotFood(core::Cell &cell, core::Entity &food_entity);

  static bool CouldSensedFood(core::Cell &cell, core::Entity &food_entity) {
    return (cell.GetPosition() - food_entity.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  template<typename T>
  core::Vector2<float> ChooseDirection(core::Cell &cell, std::unordered_map<unsigned int, T> &food_entities) {
    // todo probably it should be generic, use for all types of cell
    core::Vector2<float> direction{};
    auto closest_food = FindClosestFood(cell, food_entities);
    if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
      direction = GetRandomDirection(cell);
    } else {
      auto target_position = closest_food.value().GetPosition();
      direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    }
    return direction;
  }

  static core::Vector2<float> GetRandomDirection(core::Cell &cell);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_CELL_LOGIC_H_
