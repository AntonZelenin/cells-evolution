#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_CAMERA_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_CAMERA_H_

#include "SFML/Graphics.hpp"

namespace cells_evo::graphics {
class Camera {
 public:
  static sf::View CalculateNewView(sf::Vector2f new_size, sf::Vector2f center);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_GRAPHICS_CAMERA_H_
