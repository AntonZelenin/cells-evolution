#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"
#include "CellsEvo/world.h"
#include "CellsEvo/events.h"

namespace cells_evo::core {
struct CellsGuiState {
  uint num_alive_cells;
  uint num_hunter_cells;
  uint num_nonhunter_cells;
  uint num_dead_cells;
  uint num_food;
  float avg_frame_time_ms;
  float avg_hunter_size;
  float avg_nonhunter_size;
  float avg_hunter_speed;
  float avg_nonhunter_speed;
};

class Gui {
  static constexpr uint k_cells_history_graph_capacity_ = 70;
  float nonhunter_cells_vals_[Gui::k_cells_history_graph_capacity_]{};
  float hunter_cells_vals_[Gui::k_cells_history_graph_capacity_]{};
  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<core::World> world_;
  uint ticks_ = 0;
  uint cells_graph_max_ = 100;
  CellsGuiState cells_gui_state_{};

  std::unique_ptr<CircularQueue<uint>> frame_times_;
  std::unique_ptr<CircularQueue<uint>> hunter_cells_number_history_;
  std::unique_ptr<CircularQueue<uint>> nonhunter_cells_number_history_;

  void UpdateCellsGuiState();
  void UpdateGuiState(const sf::Clock &delta_clock, uint frame_time_ms);

 public:
  event::Dispatcher event_dispatcher_{};
  // todo should I get &world or std::shared_ptr<World>& ?
  explicit Gui(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<core::World> &world);
  ~Gui();

  void ProcessGui(sf::Clock &delta_clock, uint frame_time_ms);
  void Draw();
  static void ProcessEvent(sf::Event &event);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_
