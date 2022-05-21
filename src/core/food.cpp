#include <utility>

#include "CellsEvo/core/tools.h"
#include "CellsEvo/core/food.h"

namespace cells_evo::core {
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

void Food::SetShape(sf::RectangleShape shape) {
  shape_ = std::move(shape);
}

sf::RectangleShape &Food::GetShape() {
  return shape_;
}

FoodDeposit::FoodDeposit(Position center, float radius) {
  center_ = center;
  radius_ = radius;
}
}
