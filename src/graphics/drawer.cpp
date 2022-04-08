#include "CellsEvo/graphics.h"

namespace cells_evo::graphics {
float CellDrawer::GetThickness(float size) const {
  return size / this->k_thickness_coefficient_;
}

sf::CircleShape CellDrawer::Get(const std::shared_ptr<core::Cell> &cell) {
  sf::CircleShape shape(cell->GetSize());
  shape.setOutlineThickness(this->GetThickness(cell->GetSize()));
  shape.setOutlineColor(this->color_provider_.Get(cell->type_));
  shape.setPosition(cell->GetPosition().X() - cell->GetSize(), cell->GetPosition().Y() - cell->GetSize());
  return shape;
}

sf::Color CellColorProvider::Get(const core::Type &type) {
  return this->mapping_[type];
}

sf::RectangleShape FoodDrawer::Get(const std::shared_ptr<core::Food> &food) {
  sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
  shape.setFillColor(sf::Color::Cyan);
  shape.setPosition(static_cast<float>(food->GetPosition().X()), static_cast<float>(food->GetPosition().Y()));
  return shape;
}
}
