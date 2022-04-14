#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_

#include <vector>
#include <unordered_map>
#include "cell.h"
#include "food.h"
#include "tools.h"
#include "cell_generator.h"

namespace cells_evo::core {
typedef unsigned int Id;
// todo move to appropriate files?
typedef std::unordered_map<Id, std::shared_ptr<core::EdibleEntity>> EdibleEntityStorage;
typedef std::unordered_map<Id, std::shared_ptr<core::Cell>> CellStorage;
typedef std::unordered_map<Id, std::shared_ptr<core::Food>> FoodStorage;

class World {
  IndexDriver index_driver_{};
  FoodGenerator food_generator_{};
  core::CellGenerator cell_generator_{};

  void AddFood(std::vector<std::shared_ptr<Food>> foods);
  void AddCells(std::vector<std::shared_ptr<Cell>> new_cells);

 public:
  FoodStorage food_{};
  CellStorage cells_{};
  int width_;
  int height_;

  World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height);

  void GenerateFood(int number);
  void GenerateCells(int number);
  void GenerateHunterCells(int number);

  void AddFood(std::shared_ptr<Food> food);
  void AddCell(std::shared_ptr<Cell> cell);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_WORLD_H_
