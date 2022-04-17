#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"
#include "CellsEvo/world.h"

namespace cells_evo::core {
template<typename T>
class CircularQueue {
  uint curr_head_;
  uint size_;
  std::vector<T> vals_;

 public:
  explicit CircularQueue(uint size) {
    size_ = size;
    vals_.reserve(size_);
    curr_head_ = 0;
    for (int i = 0; i < size_; i++) {
      vals_[i] = 0;
    }
  }

  uint Size() {
    return size_;
  }

  T Get(uint i) {
    return vals_[i];
  }

  void Push(T val) {
    vals_[curr_head_++] = val;
    if (curr_head_ > size_ - 1) curr_head_ = 0;
  }

  uint GetHead() {
    return curr_head_;
  }

  uint NextIdx(uint idx) {
    return idx == (size_ - 1) ? 0 : idx + 1;
  }
};

class CellsGuiState {
 public:
  uint num_total_cells_{};
  uint num_hunter_cells_{};
  uint num_nonhunter_cells_{};
  uint num_dead_cells_{};
};

class Gui {
  const uint k_cells_history_graph_capacity_ = 50;
  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<core::World> world_;
  uint ticks_ = 0;
  CellsGuiState cells_gui_state_{};

  std::unique_ptr<CircularQueue<uint>> hunter_cells_number_history_;
  std::unique_ptr<CircularQueue<uint>> nonhunter_cells_number_history_;

  void UpdateCellsGuiState();

 public:
  // todo should I get &world or std::shared_ptr<World>& ?
  explicit Gui(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<core::World> &world);
  ~Gui();

  void ProcessGui(sf::Clock &delta_clock);
  void Draw();
  static void ProcessEvent(sf::Event &event);
};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_GUI_H_
