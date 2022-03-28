#include <chrono>
#include <thread>
#include <random>

#include "CellsEvo/core.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
void App::Run() {
  std::chrono::time_point<std::chrono::steady_clock> start;
  long long time_diff;

  while (window_->isOpen()) {
    start = std::chrono::steady_clock::now();

    sf::Event event{};
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window_->close();
    }
    logic_->WorldTick();
    window_->clear(sf::Color::Black);
    for (auto&[_, cell] : world_->cells_) {
      window_->draw(graphics::CellDrawer().Get(&cell));
    }
    for (auto&[_, cell] : world_->hunter_cells_) {
      window_->draw(graphics::CellDrawer().Get(&cell));
    }
    for (auto&[_, food] : world_->food_) {
      window_->draw(graphics::FoodDrawer::Get(&food));
    }
    window_->display();

    time_diff = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();
    if (time_diff < k_frame_micro_sec_) {
      std::this_thread::sleep_for(std::chrono::nanoseconds(k_frame_micro_sec_ - time_diff));
    }
  }
}

App::App(
    int cells_generation_size,
    int hunter_generation_size,
    int food_generation_size,
    int window_width,
    int window_height,
    int fps
) {
  window_ = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "Cells");
  window_->setVerticalSyncEnabled(true);

  world_ =
      new core::World(cells_generation_size, hunter_generation_size, food_generation_size, window_width, window_height);
  fps_ = fps;
  k_frame_micro_sec_ = 1000000 / fps;
  logic_ = new logic::Logic(*world_, fps_);
}

App::~App() {
  delete window_;
  delete logic_;
  delete world_;
}

namespace core {
static Position GenerateRandomPosition(
    int width,
    int height,
    const std::vector<Position> &occupied_positions,
    int min_distance_between_cells
) {
  auto min_dist_between_cells = static_cast<float>(min_distance_between_cells);
  Vector2<float> pos{};
  bool is_ok = false;
  while (not is_ok) {
    is_ok = true;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> x_distribution(0, width);
    std::uniform_int_distribution<int> y_distribution(0, height);
    pos = Vector2<float>(
        static_cast<float>(x_distribution(generator)),
        static_cast<float>( y_distribution(generator))
    );
    for (auto &p : occupied_positions) {
      if ((p.coordinates - pos).Magnitude() <= min_dist_between_cells) {
        is_ok = false;
        break;
      }
    }
  }
  return Position(pos);
}

std::vector<Position> GenerateRandomPositions(
    int field_width,
    int field_height,
    int size,
    int min_distance_between_positions
) {
  std::vector<Position> positions;
  positions.reserve(size);
  for (int i = 0; i < size; i++) {
    auto position = GenerateRandomPosition(
        field_width,
        field_height,
        positions,
        min_distance_between_positions
    );
    positions.push_back(position);
  }
  return positions;
}
}
}
