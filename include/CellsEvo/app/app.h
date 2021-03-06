#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_

#include "SFML/Graphics/RenderWindow.hpp"
#include "CellsEvo/core/world.h"
#include "CellsEvo/logic/logic.h"
#include "gui.h"

namespace cells_evo {
class App {
  // todo it might be better to use unique ptr, not sure now
  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<core::Gui> gui_;
  std::shared_ptr<core::World> world_;
  std::shared_ptr<logic::Logic> logic_;
  sf::Font font_;

  sf::Color background_color_ = sf::Color(15, 24, 41);

  bool draw_cells_ = true;
  bool draw_food_indices_ = false;
  bool draw_cell_indices_ = false;
  bool is_running_ = false;

  int k_frame_micro_sec_;
  int fps_;

  static bool ShouldDrawFood(core::Food &food, sf::Vector2f const &view_center, sf::Vector2f &half_view_size);
  static bool ShouldDrawCell(core::Cell &cell, sf::Vector2f const &view_center, sf::Vector2f &half_view_size);

 public:
  App(
      int nonhunter_generation_size,
      int hunter_generation_size,
      int food_generation_size,
      uint world_width,
      uint world_height,
      int fps,
      float food_production_rate_secs
  );

  void Run();
  void ProcessEvents();
  void ProcessInput();
  void Draw();
  void HandleEvent(const event::Event &e);
};
}
#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_APP_H_
