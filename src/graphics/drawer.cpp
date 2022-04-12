#include "CellsEvo/graphics.h"

namespace cells_evo::graphics {
float CellDrawer::GetThickness(float size) const {
  return size / k_thickness_coefficient_;
}

sf::CircleShape CellDrawer::Get(const std::shared_ptr<core::Cell> &cell) {
  sf::CircleShape shape(cell->GetSize());
  if (cell->IsDead()) {
    shape.setFillColor(sf::Color(130, 130, 130));
  } else {
    shape.setOutlineThickness(GetThickness(cell->GetSize()));
    shape.setOutlineColor(color_provider_.GetOutlineColor(cell));
    shape.setFillColor(sf::Color(194, 255, 254));
  }
  shape.setPosition(cell->GetPosition().x - cell->GetSize(), cell->GetPosition().y - cell->GetSize());
  return shape;
}

sf::Color CellColorProvider::GetOutlineColor(const std::shared_ptr<core::Cell> &cell) {
  return mapping_[cell->type_];
}

sf::Color FoodColorProvider::Get(const core::FoodType &type) {
  return mapping_[type];
}

sf::RectangleShape FoodDrawer::Get(const std::shared_ptr<core::Food> &food) {
  sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
  shape.setFillColor(color_provider_.Get(food->type_));
  shape.setPosition(static_cast<float>(food->GetPosition().x), static_cast<float>(food->GetPosition().y));
  return shape;
}
}
