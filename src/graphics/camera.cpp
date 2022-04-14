#include "CellsEvo/graphics/camera.h"

namespace cells_evo::graphics {

sf::View Camera::CalculateNewView(sf::Vector2f new_size, sf::Vector2f center) {
  auto new_top_left = center - (new_size / 2.f);
  return sf::View(
      sf::FloatRect(
          new_top_left,
          new_size
      )
  );
}
}
