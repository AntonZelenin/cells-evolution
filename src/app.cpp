#include <thread>
#include "CellsEvo/logic/logic.h"
#include "CellsEvo/app.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

namespace cells_evo {
// todo you can use sf::Vector2f and delta_clock -_-
void App::Run() {
  sf::Clock delta_clock;
  while (window_->isOpen()) {
    ProcessEvents();
    ProcessInput();
    ProcessGui(delta_clock);
    logic_->WorldTick();
    Draw();

    auto time_diff = delta_clock.getElapsedTime().asMicroseconds();
    if (time_diff < k_frame_micro_sec_) {
      std::this_thread::sleep_for(std::chrono::microseconds(k_frame_micro_sec_ - time_diff));
    }
    delta_clock.restart();
  }
}

void App::ProcessGui(sf::Clock &delta_clock) {
  ImGui::SFML::Update(*window_, delta_clock.getElapsedTime());
  ImGui::Begin("Sample window");
  if (ImGui::Button("Update window title")) {
    window_->setTitle("Bla bla bla");
  }
  ImGui::End();
}

void App::Draw() {
  window_->clear(background_color_);
  // todo should I filter items out of the view or sfml does it for me?
  for (auto&[_, cell] : world_->cells_) {
    window_->draw(cell_drawer_.Get(cell));
  }
  for (auto&[_, food] : world_->food_) {
    window_->draw(food_drawer_.Get(food));
  }

  ImGui::SFML::Render(*window_);
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
}

void App::ProcessEvents() {
  sf::Event event{};
  while (window_->pollEvent(event)) {
    ImGui::SFML::ProcessEvent(event);

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
    unsigned int world_width,
    unsigned int world_height,
    int fps,
    float food_production_rate_secs
) {
  const unsigned int kDefaultWidth = 1920;
  const unsigned int kDefaultHeight = 1440;
  window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(kDefaultWidth, kDefaultHeight), "Cells");
//  window_->setVerticalSyncEnabled(true);

  auto center = sf::Vector2f(static_cast<float>(world_width) / 2.f, static_cast<float>(world_height) / 2.f);
  sf::Vector2f default_view_rect = sf::Vector2f(kDefaultWidth, kDefaultHeight);
  auto top_left = center - (default_view_rect / 2.f);
  window_->setView(sf::View(sf::FloatRect(top_left, default_view_rect)));

  if (!ImGui::SFML::Init(*window_, false))
    throw std::runtime_error("Failed to init ImGui SFML");

  float scale = 2.0f;
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("include/imgui/misc/fonts/ProggyClean.ttf", scale * 16.0f);
  io.Fonts->Build();
  ImGui::GetStyle().ScaleAllSizes(scale);

  if (!ImGui::SFML::UpdateFontTexture())
    throw std::runtime_error("Failed to update ImGui::SFML font texture");

  world_ = std::make_shared<core::World>(
      cells_generation_size,
      hunter_generation_size,
      food_generation_size,
      world_width,
      world_height
  );
  fps_ = fps;
  k_frame_micro_sec_ = 1000000 / fps;
  logic_ = std::make_shared<logic::Logic>(
      *world_,
      static_cast<float>(fps_) * food_production_rate_secs
  );
}

App::~App() {
  // todo what if exception?
  ImGui::SFML::Shutdown();
}
}
