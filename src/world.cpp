#include "CellsEvo/world.h"

namespace cells_evo::core {
World::World(int cells_generation_size, int hunter_generation_size, int food_generation_size, int width, int height) {
  width_ = static_cast<float>(width);
  height_ = static_cast<float>(height);
  GenerateCells(cells_generation_size);
  GenerateHunterCells(hunter_generation_size);
  GenerateFood(food_generation_size);
}

void World::AddFood(Food &new_food) {
  uint id = index_driver_.GetNextId();
  new_food.SetId(id);
  food_.push_back(new_food);
}

void World::AddCell(Cell &cell) {
  uint id = index_driver_.GetNextId();
  cell.SetId(id);
  cells_.push_back(cell);
}

void World::GenerateFood(int number) {
  AddFood(food_generator_.CreateFloralGeneration(
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
      core::CellType::K_NONHUNTER
  ));
}

void World::GenerateHunterCells(int number) {
  AddCells(cell_generator_.Generate(
      width_,
      height_,
      number,
      core::CellType::K_HUNTER
  ));
}

void World::AddFood(std::vector<Food> foods) {
  // todo will it work as I expect?
  for (auto &f : foods) {
    AddFood(f);
  }
  SortFood();
}

void World::AddCells(std::vector<Cell> new_cells) {
  for (auto &c : new_cells) {
    AddCell(c);
  }
  SortCells();
}

bool CellLessByX(core::Cell &cell_1, core::Cell &cell_2) {
  return cell_1.GetPosition().x < cell_2.GetPosition().x;
}

bool FoodLessByX(core::Food &food_1, core::Food &food_2) {
  return food_1.GetPosition().x < food_2.GetPosition().x;
}

void World::SortCells() {
  std::sort(cells_.begin(), cells_.end(), CellLessByX);
}

void World::SortFood() {
  std::sort(food_.begin(), food_.end(), FoodLessByX);
}

void World::Tick() {
  ++ticks_;
}
}
