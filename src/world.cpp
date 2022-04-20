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
  uint id = index_driver_.GetNextId();
  new_food->SetId(id);
  tiled_field_.AddEntity(new_food);
  food_.insert({id, std::move(new_food)});
}

void World::AddCell(std::shared_ptr<Cell> cell) {
  uint id = index_driver_.GetNextId();
  cell->SetId(id);
  tiled_field_.AddEntity(cell);
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
//  AddCell(std::make_shared<Cell>(
//      core::CellType::K_NONHUNTER,
//      core::Position(2701, 1801),
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
  num_of_tiles_x_ = ceil(static_cast<float>(width) / k_tile_size_);
  num_of_tiles_y_ = ceil(static_cast<float>(height) / k_tile_size_);
  num_of_tiles_ = num_of_tiles_x_ * num_of_tiles_y_;
  tiles_.reserve(num_of_tiles_);
  for (uint i = 0; i < num_of_tiles_; i++) {
    tiles_.emplace_back();
    // warm up cache
    GetNeighbouringTileIndices(i);
  }
}

void Field::EntityMoved(const std::shared_ptr<core::Entity> &ent) {
  auto
      x = ent->GetPosition().x,
      y = ent->GetPosition().y,
      size = ent->GetSize();
  if (
      (ceil((x + size) / k_tile_size_) != ceil(((x - size) / k_tile_size_))
          && (x - size) > 0 && (x + size) < num_of_tiles_x_ * k_tile_size_)
          || (ceil((y + size) / k_tile_size_) != ceil((y - size) / k_tile_size_)
              && (y - size) > 0 && (y + size) < num_of_tiles_y_ * k_tile_size_)
      ) {
    auto eid = ent->GetId();

    auto old_tiles = entity_tiles_cache_.find(eid);
    if (old_tiles != entity_tiles_cache_.end()) {
      for (auto tile_idx : old_tiles->second) {
        auto &tile = tiles_[tile_idx];
        for (auto it = tile.begin(); it != tile.end();) {
          // here something broke, bad access
          if (it->expired()) {
            it = tile.erase(it);
          } else if (it->lock()->GetId() == eid) {
            tile.erase(it);
            break;
          } else {
            it++;
          }
        }
      }
      entity_tiles_cache_.erase(eid);
    }

    AddEntity(ent);
  }
}

std::vector<uint> Field::GetTileIndices(const std::weak_ptr<core::Entity> &entity) {
  auto ent = entity.lock();
  auto res = entity_tiles_cache_.find(ent->GetId());
  if (res != entity_tiles_cache_.end()) return res->second;

  std::vector<uint> indices;
  // you can cache cell tile until it's changed
  auto
      x = ent->GetPosition().x,
      y = ent->GetPosition().y,
      size = ent->GetSize();
  // for a cell size is radius, is it also half size for food?
  // todo I can make an inline function
  bool left_x_in_different_tile = ceil(x / k_tile_size_) != ceil(((x - size) / k_tile_size_))
      && (x - size) > 0;
  bool right_x_in_different_tile = x != 0 && ceil(x / k_tile_size_) != ceil((x + size) / k_tile_size_)
      && (x + size) < num_of_tiles_x_ * k_tile_size_;
  bool top_y_in_different_tile = ceil(y / k_tile_size_) != ceil((y - size) / k_tile_size_)
      && (y - size) > 0;
  bool bottom_y_in_different_tile = y != 0 && ceil(y / k_tile_size_) != ceil((y + size) / k_tile_size_)
      && (y + size) < num_of_tiles_y_ * k_tile_size_;

  auto base = GetCoordinatesTileIdx(x, y);
  indices.push_back(base);
  if (left_x_in_different_tile) {
    indices.push_back(base - 1);
    if (top_y_in_different_tile)
      indices.push_back(base - num_of_tiles_x_ - 1);
    if (bottom_y_in_different_tile)
      indices.push_back(base + num_of_tiles_x_ - 1);
  }
  if (right_x_in_different_tile) {
    indices.push_back(base + 1);
    if (top_y_in_different_tile)
      indices.push_back(base - num_of_tiles_x_ + 1);
    if (bottom_y_in_different_tile)
      indices.push_back(base + num_of_tiles_x_ + 1);
  }
  if (top_y_in_different_tile)
    indices.push_back(base - num_of_tiles_x_);
  if (bottom_y_in_different_tile)
    indices.push_back(base + num_of_tiles_x_);

  entity_tiles_cache_.insert({ent->GetId(), indices});

  return indices;
}

uint Field::GetCoordinatesTileIdx(float x, float y) const {
  uint x_row = x <= 0.f ? 1 : static_cast<uint>(ceil(x / k_tile_size_));
  uint y_row = y <= 0.f ? 1 : static_cast<uint>(ceil(y / k_tile_size_));
  return y_row * num_of_tiles_x_ - (num_of_tiles_x_ - x_row) - 1;
}

void Field::AddEntity(const std::weak_ptr<core::Entity> &entity) {
  auto new_tiles = GetTileIndices(entity);
  entity_tiles_cache_.insert({entity.lock()->GetId(), new_tiles});
  for (auto idx : new_tiles) {
    tiles_[idx].push_back(entity);
  }
}

// todo create a method that returns neighbour tiles depending on a provided radius, it will reduce number of tiles
std::vector<uint> Field::GetNeighbouringTileIndices(core::Entity *entity) {
  return GetNeighbouringTileIndices(
      GetCoordinatesTileIdx(entity->GetPosition().x, entity->GetPosition().y)
  );
}

std::vector<uint> Field::GetNeighbouringTileIndices(uint tile_idx) {
  // todo what if large cell sense and you need to search for more then one neighbouring tile?
  // you can use max sense distance to calculate number of tiles to return
  if (tile_neighbours_cache_.find(tile_idx) != tile_neighbours_cache_.end())
    return tile_neighbours_cache_.at(tile_idx);
  uint shifted_idx = tile_idx + 1;

  std::vector<uint> ids;
  bool is_right_border = shifted_idx % num_of_tiles_x_ == 0;
  bool is_left_border = (shifted_idx - 1) % num_of_tiles_x_ == 0;
  bool is_top_border = shifted_idx <= num_of_tiles_x_;
  bool is_bottom_border = shifted_idx > num_of_tiles_ - num_of_tiles_x_;

  if (!is_top_border) {
    if (!is_left_border)
      ids.push_back(tile_idx - num_of_tiles_x_ - 1);
    ids.push_back(tile_idx - num_of_tiles_x_);
    if (!is_right_border)
      ids.push_back(tile_idx - num_of_tiles_x_ + 1);
  }
  if (!is_left_border)
    ids.push_back(tile_idx - 1);
  ids.push_back(tile_idx);
  if (!is_right_border)
    ids.push_back(tile_idx + 1);
  if (!is_bottom_border) {
    if (!is_left_border)
      ids.push_back(tile_idx + num_of_tiles_x_ - 1);
    ids.push_back(tile_idx + num_of_tiles_x_);
    if (!is_right_border)
      ids.push_back(tile_idx + num_of_tiles_x_ + 1);
  }

  tile_neighbours_cache_.insert({tile_idx, ids});

  return ids;
}

// todo this is a hack?
void Field::Clean() {
  for (auto &tile : tiles_) {
    for (auto it = tile.begin(); it != tile.end();) {
      if (it->expired()) {
        // this thing fails sometimes
        it = tile.erase(it);
      } else {
        it++;
      }
    }
  }
}
}
