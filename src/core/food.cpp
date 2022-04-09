#include "CellsEvo/core/tools.h"
#include "CellsEvo/core/food.h"

namespace cells_evo::core {
std::vector<std::shared_ptr<Food>> FoodGenerator::Generate(
    int field_width,
    int field_height,
    int size
) const {
  std::vector<std::shared_ptr<Food>> foods;
  foods.reserve(size);
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width, field_height, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    auto food = std::make_shared<Food>(position);
    foods.push_back(food);
  }
  return foods;
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
float Food::GetRadius() const {
  return 0.1;
}
}
