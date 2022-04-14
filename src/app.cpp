#include <chrono>
#include <thread>
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/app.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
void App::Run() {
  std::chrono::time_point<std::chrono::steady_clock> start;
  long long time_diff;
  auto background_color = sf::Color(15, 24, 41);
  auto cell_drawer = graphics::CellDrawer();
  auto food_drawer = graphics::FoodDrawer();

  while (window_->isOpen()) {
    start = std::chrono::steady_clock::now();

    sf::Event event{};
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window_->close();
      if (event.type == sf::Event::Resized) {
        auto new_size = sf::Vector2f(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        window_->setView(graphics::Camera::CalculateNewView(new_size, window_->getView().getCenter()));
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        auto view = window_->getView();
        view.zoom(1 + 0.01f * event.mouseWheelScroll.delta);
        window_->setView(view);
      }
    }

    // todo move to a separate class and calculate move distance
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      auto view = window_->getView();
      view.move(0.0, -5.0);
      window_->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      auto view = window_->getView();
      view.move(-5.0, 0);
      window_->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      auto view = window_->getView();
      view.move(0.0, 5.0);
      window_->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      auto view = window_->getView();
      view.move(5.0, 0.0);
      window_->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
      auto view = window_->getView();
      view.zoom(1 + 0.01f);
      window_->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
      auto view = window_->getView();
      view.zoom(1 - 0.01f);
      window_->setView(view);
    }

    logic_->WorldTick();
    window_->clear(background_color);
    // todo should I filter items out of the view or sfml does it for me?
    for (auto&[_, cell] : world_->cells_) {
      window_->draw(cell_drawer.Get(cell));
    }
    for (auto&[_, food] : world_->food_) {
      window_->draw(food_drawer.Get(food));
    }
    window_->display();

    time_diff = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();
    if (time_diff < k_frame_micro_sec_) {
      std::this_thread::sleep_for(std::chrono::microseconds(k_frame_micro_sec_ - time_diff));
    }
  }
}

App::App(
    int cells_generation_size,
    int hunter_generation_size,
    int food_generation_size,
    int window_width,
    int window_height,
    int fps,
    float food_production_rate_secs
) {
  window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(window_width, window_height), "Cells");
//  window_->setVerticalSyncEnabled(true);

  world_ = std::make_shared<core::World>(
      cells_generation_size,
      hunter_generation_size,
      food_generation_size,
      window_width,
      window_height
  );
  fps_ = fps;
  k_frame_micro_sec_ = 1000000 / fps;
  logic_ = std::make_shared<logic::Logic>(
      *world_,
      static_cast<float>(fps_) * food_production_rate_secs
  );
}
}
