#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_DRAWER_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_DRAWER_H_

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/cell.h"
#include "CellsEvo/food.h"

namespace cells_evo::graphics {
class CellColorProvider {
  std::unordered_map<core::CellType, sf::Color> mapping_{
      {core::CellType::K_HUNTER, sf::Color::Red},
      {core::CellType::K_NONHUNTER, sf::Color::Green},
//      {core::CellType::K_NONHUNTER, sf::Color(17, 132, 194)},
  };

 public:
  sf::Color GetOutlineColor(const std::shared_ptr<core::Cell> &cell);
};

class CellDrawer {

  const float k_thickness_coefficient_ = 3.0;
  CellColorProvider color_provider_{};

  [[nodiscard]] float GetThickness(const std::shared_ptr<core::Cell> &cell) const;
  [[nodiscard]] float GetSize(const std::shared_ptr<core::Cell> &cell) const;
  [[nodiscard]] float GetBaseThickness(const std::shared_ptr<core::Cell> &cell) const;

 public:
  sf::CircleShape Get(const std::shared_ptr<core::Cell> &cell);
};

class FoodColorProvider {
  std::unordered_map<core::FoodType, sf::Color> mapping_{
      {core::FoodType::K_FLORAL, sf::Color(13, 107, 38)},
      {core::FoodType::K_ANIMAL, sf::Color::Red},
  };

 public:
  sf::Color Get(const core::FoodType &type);
};

// todo make interface for drawers
class FoodDrawer {
  FoodColorProvider color_provider_{};

 public:
  sf::RectangleShape Get(const std::shared_ptr<core::Food> &food);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_DRAWER_H_
