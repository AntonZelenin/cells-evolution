#include "CellsEvo/tools.h"
#include "CellsEvo/food.h"

namespace cells_evo::core {
std::vector<Food> FoodGenerator::CreateFloralGeneration(
    int field_width,
    int field_height,
    int size
) const {
  std::vector<Food> foods;
  foods.reserve(size);
  auto positions = GenerateRandomPositions(
      field_width, field_height, size, k_min_distance_between_items_
  );
  for (auto &position : positions) {
    Food food(FoodType::K_FLORAL, position);
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
  return nutrition_value * 2.f;
}

Position &Food::GetPosition() {
  return position_;
}

uint Food::GetId() const {
  return id_;
}

void Food::SetId(uint id) {
  id_ = id;
}

float Food::GetSize() const {
  return radius_;
}

float Food::GetNutritionValue() const {
  return nutrition_value_;
}

bool Food::IsDeleted() const {
  return is_deleted_;
}

void Food::Delete() {
  is_deleted_ = true;
}
}
