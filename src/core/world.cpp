#include "CellsEvo/core/world.h"

namespace cells_evo::core {
World::World() {
  this->width_ = 1920;
  this->height_ = 1080;
  this->GenerateFood(20);
  this->GenerateCells(5);
}

void World::AddFood(Food new_food) {
  unsigned int id = this->index_driver_.GetNextId();
  new_food.SetId(id);
  this->food_.insert({id, new_food});
}

void World::AddCell(Cell cell) {
  unsigned int id = this->index_driver_.GetNextId();
  cell.SetId(id);
  this->cells_.insert({id, cell});
}

void World::GenerateFood(int number) {
  this->AddFood(this->food_generator_.Generate(
      this->width_,
      this->height_,
      number
  ));
}

void World::GenerateCells(int number) {
  this->AddCells(this->cell_generator_.Generate(
      this->width_,
      this->height_,
      number
  ));
}

void World::AddFood(const std::vector<Food> &foods) {
  // todo will it work as I expect?
  for (const auto &f : foods) {
    this->AddFood(f);
  }
}

void World::AddCells(const std::vector<Cell> &new_cells) {
  for (const auto &c : new_cells) {
    this->AddCell(c);
  }
}
}
