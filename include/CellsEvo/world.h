#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_

#include <vector>
#include "cell.h"
#include "food.h"
#include "tools.h"
#include "cell_generator.h"

namespace cells_evo::core {
typedef std::vector<core::Cell> CellStorage;
typedef std::vector<core::Food> FoodStorage;

class World {
  IndexDriver index_driver_{};
  FoodGenerator food_generator_{};
  core::CellGenerator cell_generator_{};

  void AddFood(std::vector<Food> foods);
  void AddCells(std::vector<Cell> new_cells);

 public:
  FoodStorage food_{};
  CellStorage cells_{};
  float width_;
  float height_;

  World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height);

  void GenerateFood(int number);
  void GenerateCells(int number);
  void GenerateHunterCells(int number);

  void AddFood(Food &food);
  void AddCell(Cell &cell);
  void SortCells();
  void SortFood();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
