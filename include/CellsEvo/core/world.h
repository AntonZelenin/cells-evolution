#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_

#include <vector>
#include <unordered_map>
#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/food.h"
#include "tools.h"
#include "cell_generator.h"

namespace cells_evo::core {
class World {
  IndexDriver index_driver_{};
  FoodGenerator food_generator_{};
  core::CellGenerator cell_generator_{};

  void AddFood(Food food);
  void AddFood(const std::vector<Food> &foods);

  void AddCells(const std::vector<Cell> &new_cells);
  void AddHunterCells(const std::vector<Cell> &new_cells);

 public:
  std::unordered_map<unsigned int, Food> food_{};
  std::unordered_map<unsigned int, Cell> cells_{};
  std::unordered_map<unsigned int, Cell> hunter_cells_{};
  int width_;
  int height_;

  World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height);

  void GenerateFood(int number);
  void GenerateCells(int number);
  void GenerateHunterCells(int number);

  void AddCell(Cell cell);
  void AddHunterCell(Cell cell);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_
