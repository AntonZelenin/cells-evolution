#include "CellsEvo/graphics.h"

namespace cells_evo::graphics {
float CellDrawer::GetThickness(float size) const {
  return size / this->k_thickness_coefficient_;
}

sf::CircleShape CellDrawer::Get(const std::shared_ptr<core::Cell> &cell) {
  sf::CircleShape shape(cell->GetRadius());
  shape.setFillColor(sf::Color(194, 255, 254));
  shape.setOutlineThickness(this->GetThickness(cell->GetRadius()));
  shape.setOutlineColor(this->color_provider_.Get(cell->type_));
  shape.setPosition(cell->GetPosition().x - cell->GetRadius(), cell->GetPosition().y - cell->GetRadius());
  return shape;
}

sf::Color CellColorProvider::Get(const core::Type &type) {
  return this->mapping_[type];
}

sf::RectangleShape FoodDrawer::Get(const std::shared_ptr<core::Food> &food) {
  sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
  shape.setFillColor(sf::Color::Cyan);
  shape.setPosition(static_cast<float>(food->GetPosition().x), static_cast<float>(food->GetPosition().y));
  return shape;
}
}
