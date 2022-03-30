#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_

#include "cell.h"
#include "world.h"

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
  ) {
    for (auto&[_, cell] : cells) {
      auto direction = ChooseDirection(cell, food);
      Move(cell, direction, cell.speed_);
    }
  }

  core::Vector2<float> ChooseDirection(core::Cell &cell, std::map<unsigned int, core::Food> &food) {
    core::Vector2<float> direction{};
    auto closest_food = FindClosestFood(cell, food);
    if (!closest_food || !CouldSensedFood(cell, closest_food.value())) {
      direction = GetRandomSinDirection();
    } else {
      auto target_position = closest_food.value().GetPosition();
      direction = core::GetDirectionVector(cell.GetPosition(), target_position);
    }
    return direction;
  }

  // todo I want it to return a reference, do I?
  std::optional<core::Food> FindClosestFood(core::Cell &cell, std::map<unsigned int, core::Food> &foods) {
    if (foods.empty())
      return {};
    auto cached_closest_food = cell_food_cache_.find(cell.id_);
    if (cached_closest_food != cell_food_cache_.end()) {
      auto food_id = cached_closest_food->second;
      auto result = foods.find(food_id);
      if (result != foods.end())
        return result->second;
      else
        cell_food_cache_.erase(food_id);
    }

    unsigned int closest_food_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, food] : foods) {
      // todo seems position makes life harder
      auto dist = (food.GetPosition() - cell.GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_food_idx = idx;
      }
    }

    cell_food_cache_.insert({cell.id_, closest_food_idx});
    return foods.find(closest_food_idx)->second;
  }

  static bool CouldSensedFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() <= core::Cell::k_max_distance_food_detection_;
  }

  core::Vector2<float> GetRandomSinDirection() {
    // todo here
    return {1.0, 0.5};
  }

  void RebuildCellsFoodCache(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  ) {
    cell_food_cache_.clear();
    for (auto&[_, cell] : cells) {
      FindClosestFood(cell, food);
    }
  }

  void ProcessEatFood(
      std::map<unsigned int, core::Cell> &cells,
      std::map<unsigned int, core::Food> &food
  ) {
    for (auto&[_, cell] : cells) {
      auto closest_food = FindClosestFood(cell, food);
      if (closest_food && CellGotFood(cell, closest_food.value())) {
        cell.AddEnergy(core::Food::k_default_energy_value_);
        food.erase(closest_food.value().id_);
      }
    }
  }

  static bool CellGotFood(core::Cell &cell, core::Food &food) {
    return (cell.GetPosition() - food.GetPosition()).Magnitude() < cell.size_;
  }
};

class HunterLogic {
  std::map<unsigned int, unsigned int> hunter_cell_cell_cache_;

  // todo looks like duplicate
  std::optional<core::Cell> FindClosestCell(core::Cell &hunter_cell, std::map<unsigned int, core::Cell> &cells) {
    if (cells.empty())
      return {};
    auto cached_closest_cell = hunter_cell_cell_cache_.find(hunter_cell.id_);
    if (cached_closest_cell != hunter_cell_cell_cache_.end()) {
      auto hunter_cell_id = cached_closest_cell->first;
      auto cell_id = cached_closest_cell->second;
      auto cell = cells.find(cell_id);
      if (cell != cells.end())
        return cell->second;
      else
        hunter_cell_cell_cache_.erase(hunter_cell_id);
    }

    unsigned int closest_cell_idx;
    float min_distance = std::numeric_limits<float>::max();
    for (auto&[idx, cell] : cells) {
      auto dist = (cell.GetPosition() - hunter_cell.GetPosition()).Magnitude();
      if (dist < min_distance) {
        min_distance = dist;
        closest_cell_idx = idx;
      }
    }

    hunter_cell_cell_cache_.insert({hunter_cell.id_, closest_cell_idx});
    return cells.find(closest_cell_idx)->second;
  }
 public:
  void MoveCells(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells,
      unsigned int world_tick
  ) {
    // update once in 10 frames
    if (world_tick % 10 == 0) hunter_cell_cell_cache_.clear();
    for (auto&[_, hunter_cell] : hunter_cells) {
      auto closest_cell = FindClosestCell(hunter_cell, cells);
      if (!closest_cell) return;
      auto target_position = closest_cell.value().GetPosition();
      auto direction = core::GetDirectionVector(hunter_cell.GetPosition(), target_position);
      Move(hunter_cell, direction, hunter_cell.speed_);
    }
  }

  void ProcessEatCell(
      std::map<unsigned int, core::Cell> &hunter_cells,
      std::map<unsigned int, core::Cell> &cells
  ) {
    for (auto&[_, hunter_cell] : hunter_cells) {
      auto cell = FindClosestCell(hunter_cell, cells);
      if (cell && HunterCellGotCell(hunter_cell, cell.value())) {
        hunter_cell.AddEnergy(core::Cell::k_default_energy_value_);
        // todo this is not good
        cells.erase(cell.value().id_);
      }
    }
  }

  // todo not duplicate?
  static bool HunterCellGotCell(core::Cell &hunter_cell, core::Cell &cell) {
    return (hunter_cell.GetPosition() - cell.GetPosition()).Magnitude() < (hunter_cell.size_ + cell.size_);
  }
};

class Logic {
  core::World &world_;
  unsigned int world_ticks_ = 0;
  unsigned int food_production_rate_;

  HunterLogic hunter_logic_;
  NonHunterLogic non_hunter_logic_;

  void CountTick();

  void Eat();

  void GenerateFood();

  void DivideCells();

  static core::Cell DivideCell(core::Cell &cell);

  void CheckCellsEnergy();

 public:
  explicit Logic(core::World &world, unsigned int food_production_rate);

  void WorldTick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_LOGIC_H_
