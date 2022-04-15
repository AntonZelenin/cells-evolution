#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_

#include "SFML/Graphics/RenderWindow.hpp"
#include "world.h"
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/graphics/drawer.h"

namespace cells_evo {
class App {
  // todo it might be better to use unique ptr, not sure now
  std::shared_ptr<sf::RenderWindow> window_;

  std::shared_ptr<core::World> world_;
  std::shared_ptr<logic::Logic> logic_;

  graphics::CellDrawer cell_drawer_{};
  graphics::FoodDrawer food_drawer_{};

  sf::Color background_color_ = sf::Color(15, 24, 41);

  int k_frame_micro_sec_;
  int fps_;
 public:
  App(
      int cells_generation_size,
      int hunter_generation_size,
      int food_generation_size,
      unsigned int world_width,
      unsigned int world_height,
      int fps,
      float food_production_rate_secs
  );
  ~App();

  void Run();
  void ProcessEvents();
  void ProcessInput();
  void Draw();
  void ProcessGui(sf::Clock &delta_clock);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_
