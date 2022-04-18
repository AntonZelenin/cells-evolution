#include "CellsEvo/world.h"

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
//  AddFood(
//      std::make_shared<Food>(FoodType::K_FLORAL, core::Position(0, 0))
//  );
  AddFood(food_generator_.CreateFloralGeneration(
      width_,
      height_,
      number
  ));
}

void World::GenerateCells(int number) {
//  AddEntity(std::make_shared<Cell>(
//      core::CellType::K_NONHUNTER,
//      core::Position(0, 0),
//      cell_generator_.genetic_engineer_.GenerateBaseGenes(core::CellType::K_NONHUNTER)
//  ));
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

Field::Field(uint width, uint height) {
  num_of_tiles_x_ = static_cast<uint>(ceil(width / k_tile_size_));
  num_of_tiles_y_ = static_cast<uint>(ceil(height / k_tile_size_));
  num_of_tiles_ = num_of_tiles_x_ * num_of_tiles_y_;
  tiles_.reserve(num_of_tiles_);
  for (uint i = 0; i < num_of_tiles_; i++) {
    // warm up cache
    GetNeighbouringTileIndices(i);
  }
}

uint Field::GetTileIdx(const std::weak_ptr<core::Entity> &entity) const {
  uint x_tile_num = static_cast<uint>(static_cast<uint>(entity.lock()->GetPosition().x) / num_of_tiles_x_);
  uint y_tile_num = static_cast<uint>(static_cast<uint>(entity.lock()->GetPosition().y) / num_of_tiles_y_);
  return y_tile_num * num_of_tiles_x_ - (num_of_tiles_x_ - x_tile_num);
}

void Field::AddEntity(const std::weak_ptr<core::Entity> &entity) {
  tiles_[GetTileIdx(entity)].push_back(std::weak_ptr<Entity>(entity));
}

std::vector<uint> Field::GetNeighbouringTileIndices(std::weak_ptr<core::Entity> &entity) {
  return GetNeighbouringTileIndices(GetTileIdx(entity));
}

std::vector<uint> Field::GetNeighbouringTileIndices(uint tile_idx) {
  if (tile_neighbours_cache_.find(tile_idx) != tile_neighbours_cache_.end()) return tile_neighbours_cache_.at(tile_idx);

  std::vector<uint> ids;
  bool is_right_border = tile_idx == 0 || ((tile_idx - 1) % num_of_tiles_x_) == 0;
  bool is_left_border = tile_idx % num_of_tiles_x_ == 0;
  bool is_top_border = tile_idx < num_of_tiles_x_;
  bool is_bottom_border = tile_idx >= num_of_tiles_ - num_of_tiles_x_;

  if (!is_left_border && !is_top_border) ids.push_back(tile_idx + num_of_tiles_x_ - 1);
  if (!is_top_border) ids.push_back(tile_idx + num_of_tiles_x_);
  if (!is_right_border && !is_top_border) ids.push_back(tile_idx + num_of_tiles_x_ + 1);
  if (!is_left_border) ids.push_back(tile_idx + 1);
  ids.push_back(tile_idx);
  if (!is_right_border) ids.push_back(tile_idx - 1);
  if (!is_left_border && !is_bottom_border) ids.push_back(tile_idx - num_of_tiles_x_ - 1);
  if (!is_bottom_border) ids.push_back(tile_idx - num_of_tiles_x_);
  if (!is_right_border && !is_bottom_border) ids.push_back(tile_idx - num_of_tiles_x_ + 1);

  tile_neighbours_cache_.insert({tile_idx, ids});

  return ids;
  // todo test it
}
}
