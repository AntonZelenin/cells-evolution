#include "CellsEvo/tools.h"
#include "CellsEvo/food.h"

namespace cells_evo::core {
std::vector<std::shared_ptr<Food>> FoodGenerator::CreateFloralGeneration(
    int field_width,
    int field_height,
    int size
) const {
  std::vector<std::shared_ptr<Food>> foods;
  foods.reserve(size);
  // -10 - don't put food too close to borders, cells get stuck
  // todo but hunters still stuck
  auto positions = cells_evo::core::GenerateRandomPositions(
      field_width - 10, field_height - 10, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    auto food = std::make_shared<Food>(FoodType::K_FLORAL, position);
    foods.push_back(food);
  }
  return foods;
}

Food::Food(FoodType type, Position position) : position_(position) {
  type_ = type;
  nutrition_value_ = 5;
  radius_ = CalculateRadius(nutrition_value_);
}

Food::Food(FoodType type, Position position, float nutrition_value) : position_(position) {
  type_ = type;
  radius_ = CalculateRadius(nutrition_value);
  nutrition_value_ = 5;
}

float Food::CalculateRadius(float nutrition_value) {
  return nutrition_value * 0.02f;
}

Position &Food::GetPosition() {
  return position_;
}

unsigned int Food::GetId() const {
  return id_;
}

void Food::SetId(unsigned int id) {
  id_ = id;
}

void Food::SetPosition(Position pos) {
  position_ = pos;
}

float Food::GetSize() const {
  return radius_;
}

float Food::GetNutritionValue() const {
  return nutrition_value_;
}
}
