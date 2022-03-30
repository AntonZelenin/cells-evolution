#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_

#include "SFML/Graphics/RenderWindow.hpp"
#include "cell.h"
#include "food.h"
#include "world.h"
#include "CellsEvo/core/logic/logic.h"

namespace cells_evo {
class App {
  sf::RenderWindow *window_;

  core::World *world_;
  logic::Logic *logic_;

  int k_frame_micro_sec_;
  int fps_;
 public:
  App(
      int cells_generation_size,
      int hunter_generation_size,
      int food_generation_size,
      int window_width,
      int window_height,
      int fps,
      float food_production_rate
  );
  ~App();

  void Run();
};

namespace core {
// todo find it a better place?
std::vector<Position> GenerateRandomPositions(
    int field_width,
    int field_height,
    int size,
    int min_distance_between_positions
);
}
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_
