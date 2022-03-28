#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_

#include <vector>
#include <map>
#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/food.h"
#include "tools.h"

namespace cells_evo::core {
class World {
  // todo do I need to delete these in destructor?
  IndexDriver index_driver_{};
  FoodGenerator food_generator_{};
  CellGenerator cell_generator_{};

  void AddCell(Cell cell);

  void AddHunterCell(Cell cell);

  void AddFood(Food food);

  void AddFood(const std::vector<Food> &foods);

  void AddCells(const std::vector<Cell> &new_cells);

  void AddHunterCells(const std::vector<Cell> &new_cells);

 public:
  std::map<unsigned int, Food> food_{};
  std::map<unsigned int, Cell> cells_{};
  std::map<unsigned int, Cell> hunter_cells_{};
  int width_;
  int height_;

  World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height);

  void GenerateFood(int number);

  void GenerateCells(int number);

  void GenerateHunterCells(int number);
};

core::Entity *FindNearest(Position position, std::vector<core::Entity>);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_WORLD_H_
