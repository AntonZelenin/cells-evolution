#include <utility>
#include "CellsEvo/gui.h"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include "imgui/imgui_demo.cpp"

namespace cells_evo::core {
Gui::Gui(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<core::World> &world) : world_(world) {
  window_ = std::move(window);
  cells_gui_state_ = CellsGuiState();
  hunter_cells_number_history_ = std::make_unique<CircularQueue<uint>>(Gui::k_cells_history_graph_capacity_);
  nonhunter_cells_number_history_ = std::make_unique<CircularQueue<uint>>(Gui::k_cells_history_graph_capacity_);

  if (!ImGui::SFML::Init(*window_, false))
    throw std::runtime_error("Failed to init ImGui SFML");

  // todo determine scale dynamically
  float scale = 2.0f;
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("include/imgui/misc/fonts/ProggyClean.ttf", scale * 16.0f);
  io.Fonts->Build();
  ImGui::GetStyle().ScaleAllSizes(scale);

  if (!ImGui::SFML::UpdateFontTexture())
    throw std::runtime_error("Failed to update ImGui::SFML font texture");
}

void Gui::ProcessGui(sf::Clock &delta_clock) {
  // todo, make it better
  if (ticks_++ % 20 == 0) {
    UpdateCellsGuiState();
  }
  if (ticks_ % 120 == 0) {
    ticks_ = 0;
    hunter_cells_number_history_->Push(cells_gui_state_.num_hunter_cells_);
    nonhunter_cells_number_history_->Push(cells_gui_state_.num_nonhunter_cells_);
    cells_graph_max_ = cells_gui_state_.num_alive_cells_;
  }

  ImGui::SFML::Update(*window_, delta_clock.getElapsedTime());
  ImGui::Begin("Cells statistics");

  // todo this is awful, improve
  float nonhunter_cells_vals[Gui::k_cells_history_graph_capacity_];
  float hunter_cells_vals[Gui::k_cells_history_graph_capacity_];
  uint idx = nonhunter_cells_number_history_->GetHead();
  uint idx2 = hunter_cells_number_history_->GetHead();
  for (int i = 0; i < nonhunter_cells_number_history_->Size(); i++) {
    nonhunter_cells_vals[i] = static_cast<float>(nonhunter_cells_number_history_->Get(idx));
    idx = nonhunter_cells_number_history_->NextIdx(idx);
    hunter_cells_vals[i] = static_cast<float>(hunter_cells_number_history_->Get(idx2));
    idx2 = hunter_cells_number_history_->NextIdx(idx2);
  }

  // todo graph size should be also dynamically calculated
  ImGui::Text("Non-hunter cells: %u", cells_gui_state_.num_nonhunter_cells_);
  ImGui::PlotHistogram(
      "",
      reinterpret_cast<const float *>(&nonhunter_cells_vals),
      static_cast<int>(Gui::k_cells_history_graph_capacity_),
      0,
      nullptr,
      0.0f,
      static_cast<float>(cells_graph_max_),
      ImVec2(400, 120.0f)
  );
  ImGui::Text("Hunter cells: %u", cells_gui_state_.num_hunter_cells_);
  ImGui::PlotHistogram(
      "",
      reinterpret_cast<const float *>(&hunter_cells_vals),
      static_cast<int>(Gui::k_cells_history_graph_capacity_),
      0,
      nullptr,
      0.0f,
      static_cast<float>(cells_graph_max_),
      ImVec2(400, 120.0f)
  );
  ImGui::Text("Alive cells: %u", cells_gui_state_.num_alive_cells_);
  ImGui::Text("Dead cells: %u", cells_gui_state_.num_dead_cells_);
  ImGui::End();
}

void Gui::UpdateCellsGuiState() {
  cells_gui_state_ = CellsGuiState();
  for (auto &[_, cell] : world_->cells_) {
    if (cell->IsDead())
      cells_gui_state_.num_dead_cells_++;
    else {
      cells_gui_state_.num_alive_cells_++;
      if (cell->IsHunter())
        cells_gui_state_.num_hunter_cells_++;
      else
        cells_gui_state_.num_nonhunter_cells_++;
    }
  }
}

void Gui::Draw() {
//  ImGui::ShowDemoWindow();
  ImGui::SFML::Render(*window_);
}

void Gui::ProcessEvent(sf::Event &event) {
  ImGui::SFML::ProcessEvent(event);
}

Gui::~Gui() {
  // todo what if exception?
  ImGui::SFML::Shutdown();
}
}
