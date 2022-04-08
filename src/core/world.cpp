#include "CellsEvo/core/world.h"

namespace cells_evo::core {
World::World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height) {
  width_ = width;
  height_ = height;
  GenerateCells(cells_generation_size);
  GenerateHunterCells(hunter_generation_size);
  GenerateFood(food_generation_size);
}

void World::AddFood(Food new_food) {
  unsigned int id = index_driver_.GetNextId();
  new_food.SetId(id);
  food_.insert({id, new_food});
}

void World::AddCell(Cell cell) {
  unsigned int id = index_driver_.GetNextId();
  cell.SetId(id);
  cells_.insert({id, cell});
}

void World::GenerateFood(int number) {
  AddFood(food_generator_.Generate(
      width_,
      height_,
      number
  ));
}

void World::GenerateCells(int number) {
  AddCells(cell_generator_.Generate(
      width_,
      height_,
      number,
      core::Type::K_NONHUNTER
  ));
}

void World::GenerateHunterCells(int number) {
  AddCells(cell_generator_.Generate(
      width_,
      height_,
      number,
      core::Type::K_HUNTER
  ));
}

void World::AddFood(const std::vector<Food> &foods) {
  // todo will it work as I expect?
  for (const auto &f : foods) {
    AddFood(f);
  }
}

void World::AddCells(const std::vector<Cell> &new_cells) {
  for (const auto &c : new_cells) {
    AddCell(c);
  }
}
}
