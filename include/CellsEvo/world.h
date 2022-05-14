#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_

#include <vector>
#include "cell.h"
#include "food.h"
#include "tools.h"

namespace cells_evo::core {
typedef std::vector<core::Cell> CellStorage;
typedef std::vector<core::Food> FoodStorage;

class World {
  IndexDriver index_driver_{};

 public:
  FoodStorage food_{};
  CellStorage cells_{};
  float width_;
  float height_;
  uint ticks_ = 0;

  World(int width, int height);

  void AddFood(Food &food);
  void AddFood(std::vector<Food> foods);
  void AddCell(Cell &cell);
  void AddCells(std::vector<Cell> new_cells);
  void SortCells();
  void SortFood();
  void Tick();
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
