#include "CellsEvo/core/world.h"

namespace cells_evo::core {
World::World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height) {
  width_ = width;
  height_ = height;
  GenerateCells(cells_generation_size);
  GenerateHunterCells(hunter_generation_size);
  GenerateFood(food_generation_size);
}

void World::AddFood(std::shared_ptr<Food> new_food) {
  unsigned int id = index_driver_.GetNextId();
  new_food->SetId(id);
  food_.insert({id, std::move(new_food)});
}

void World::AddCell(std::shared_ptr<Cell> cell) {
  unsigned int id = index_driver_.GetNextId();
  cell->SetId(id);
  cells_.insert({id, std::move(cell)});
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

void World::AddFood(std::vector<std::shared_ptr<Food>> foods) {
  // todo will it work as I expect?
  for (auto &f : foods) {
    AddFood(std::move(f));
  }
}

void World::AddCells(std::vector<std::shared_ptr<Cell>> new_cells) {
  for (auto &c : new_cells) {
    AddCell(std::move(c));
  }
}
}
