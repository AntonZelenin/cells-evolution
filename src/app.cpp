#include <thread>
#include <string>
#include "CellsEvo/app.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
void App::Run() {
  uint last_frame_time = 1;
  sf::Clock delta_clock, frame_clock, fc;
  int c = 1000;
  while (window_->isOpen()) {
    --c;
    frame_clock.restart();
    fc.restart();
    ProcessEvents();
    auto events = fc.restart().asMicroseconds();
    ProcessInput();
    auto input = fc.restart().asMicroseconds();
    gui_->ProcessGui(delta_clock, last_frame_time);
    auto gui = fc.restart().asMicroseconds();
    if (run_simulation_)
      logic_->WorldTick();
    auto world_tick = fc.restart().asMicroseconds();
    Draw();
    auto draw = fc.restart().asMicroseconds();

    if (c == 0) {
      int t = 1;
    }

    auto time_diff = delta_clock.getElapsedTime().asMicroseconds();
    last_frame_time = frame_clock.getElapsedTime().asMilliseconds();
    if (time_diff < k_frame_micro_sec_)
      std::this_thread::sleep_for(std::chrono::microseconds(k_frame_micro_sec_ - time_diff));
    delta_clock.restart();
  }
}

bool App::ShouldDrawFood(core::Food &food, sf::Vector2f const &view_center, sf::Vector2f &half_view_size) {
  // todo take into account food size
  auto &pos = food.GetPosition();
  return !food.IsDeleted()
      && pos.x > view_center.x - (half_view_size.x) && pos.x < view_center.x + (half_view_size.x)
      && pos.y > view_center.y - (half_view_size.y) && pos.y < view_center.y + (half_view_size.y);
}

bool App::ShouldDrawCell(core::Cell &cell, sf::Vector2f const &view_center, sf::Vector2f &half_view_size) {
  // todo take into account cell size
  auto &pos = cell.GetPosition();
  return !cell.IsDeleted()
      && pos.x > view_center.x - (half_view_size.x) && pos.x < view_center.x + (half_view_size.x)
      && pos.y > view_center.y - (half_view_size.y) && pos.y < view_center.y + (half_view_size.y);
}

void App::Draw() {
  window_->clear(background_color_);
  auto &center = window_->getView().getCenter();
  auto half_size = window_->getView().getSize();
  half_size.x /= 2;
  half_size.y /= 2;
  // todo
  sf::Font font;
  if (!font.loadFromFile("include/imgui/misc/fonts/ProggyClean.ttf")) {
    throw std::runtime_error("Failed to load font for SFML");
  }
  for (int i = 0; auto &food : world_->food_) {
    if (ShouldDrawFood(food, center, half_size)) {
      window_->draw(food_drawer_.Get(food));
      if (draw_food_indices_) {
        sf::Text idx_label;
        idx_label.setFont(font);
        idx_label.setString(std::to_string(i));
        idx_label.setPosition(food.GetPosition().x, food.GetPosition().y);
        idx_label.setCharacterSize(20);
        idx_label.setFillColor(sf::Color::White);
        window_->draw(idx_label);
      }
    }
    ++i;
  }
  if (draw_cells_) {
    for (int i = 0; auto &cell : world_->cells_) {
      if (ShouldDrawCell(cell, center, half_size)) {
        window_->draw(cell_drawer_.Get(cell));
        if (draw_cell_indices_) {
          sf::Text idx_label;
          idx_label.setFont(font);
          idx_label.setString(std::to_string(i));
          idx_label.setPosition(cell.GetPosition().x, cell.GetPosition().y);
          idx_label.setCharacterSize(20);
          idx_label.setFillColor(sf::Color::White);
          window_->draw(idx_label);
        }
      }
      ++i;
    }
  }
  gui_->Draw();
  window_->display();
}

void App::ProcessInput() {
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
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    auto view = window_->getView();
    view.zoom(1 + 0.01f);
    window_->setView(view);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    auto view = window_->getView();
    view.zoom(1 - 0.01f);
    window_->setView(view);
  }
}

void App::ProcessEvents() {
  sf::Event event{};
  while (window_->pollEvent(event)) {
    gui_->ProcessEvent(event);

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
}

App::App(
    int cells_generation_size,
    int hunter_generation_size,
    int food_generation_size,
    uint world_width,
    uint world_height,
    int fps,
    float food_production_rate_secs
) {
  const uint kDefaultWidth = 1920;
  const uint kDefaultHeight = 1440;
  window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(kDefaultWidth, kDefaultHeight), "Cells");
//  window_->setVerticalSyncEnabled(true);

  auto center = sf::Vector2f(static_cast<float>(world_width) / 2.f, static_cast<float>(world_height) / 2.f);
  sf::Vector2f default_view_rect = sf::Vector2f(kDefaultWidth, kDefaultHeight);
  auto top_left = center - (default_view_rect / 2.f);
  window_->setView(sf::View(sf::FloatRect(top_left, default_view_rect)));

  world_ = std::make_shared<core::World>(
      cells_generation_size,
      hunter_generation_size,
      food_generation_size,
      world_width,
      world_height
  );

  gui_ = std::make_shared<core::Gui>(window_, world_);

  fps_ = fps;
  k_frame_micro_sec_ = 1000000 / fps;
  logic_ = std::make_shared<logic::Logic>(
      *world_,
      static_cast<float>(fps_) * food_production_rate_secs
  );

  gui_->event_dispatcher_.Subscribe(
      event::ToggleCellsDrawingEvent::descriptor_,
      [this](auto &&ph_1) { HandleEvent(std::forward<decltype(ph_1)>(ph_1)); }
  );
  gui_->event_dispatcher_.Subscribe(
      event::ToggleSimulation::descriptor_,
      [this](auto &&ph_1) { HandleEvent(std::forward<decltype(ph_1)>(ph_1)); }
  );
  gui_->event_dispatcher_.Subscribe(
      event::ToggleDrawCellIndices::descriptor_,
      [this](auto &&ph_1) { HandleEvent(std::forward<decltype(ph_1)>(ph_1)); }
  );
  gui_->event_dispatcher_.Subscribe(
      event::ToggleDrawFoodIndices::descriptor_,
      [this](auto &&ph_1) { HandleEvent(std::forward<decltype(ph_1)>(ph_1)); }
  );
  gui_->event_dispatcher_.Subscribe(
      event::GenerateHunterCell::descriptor_,
      [this](auto &&ph_1) { HandleEvent(std::forward<decltype(ph_1)>(ph_1)); }
  );
}

void App::HandleEvent(const event::Event &e) {
  auto event = e.Type();
  if (event == event::ToggleCellsDrawingEvent::descriptor_) {
    draw_cells_ = !draw_cells_;
  } else if (event == event::ToggleSimulation::descriptor_) {
    run_simulation_ = !run_simulation_;
  } else if (event == event::ToggleDrawCellIndices::descriptor_) {
    draw_cell_indices_ = !draw_cell_indices_;
  } else if (event == event::ToggleDrawFoodIndices::descriptor_) {
    draw_food_indices_ = !draw_food_indices_;
  } else if (event == event::GenerateHunterCell::descriptor_) {
    world_->GenerateHunterCells(1);
  }
}
}
