#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_

#include "SFML/Graphics/RenderWindow.hpp"
#include "world.h"
#include "logic/logic.h"

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
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_APP_H_
