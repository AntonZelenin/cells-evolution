#include "CellsEvo/core.h"
#include "CellsEvo/core/food.h"

#include <random>

namespace cells_evo::core {
std::vector<Food> FoodGenerator::Generate(
    int field_width,
    int field_height,
    int size = FoodGenerator::k_first_generation_size_
) const {
  std::vector<Food> food;
  food.reserve(size);
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width, field_height, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    food.emplace_back(position);
  }
  return food;
}

Food::Food(Position position) : position_(position) {
  id_ = 0;
}

Position &Food::GetPosition() {
  return position_;
}

unsigned int Food::GetId() {
  return id_;
}

void Food::SetId(unsigned int id) {
  id_ = id;
}

void Food::SetPosition(Position pos) {
  position_ = pos;
}
}
