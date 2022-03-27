#include <chrono>
#include <thread>
#include <random>

#include "CellsEvo/core.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
    void App::Run() {
        std::chrono::time_point<std::chrono::steady_clock> start;
        auto frame_micro_sec = 16666;
        long long time_diff;

        while (this->window->isOpen()) {
            start = std::chrono::steady_clock::now();

            sf::Event event{};
            while (this->window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    this->window->close();
            }
            this->logic->WorldTick();
            this->window->clear(sf::Color::Black);
            for (auto& [_, cell]: this->world->cells) {
                this->window->draw(graphics::CellDrawer().Get(&cell));
            }
            for (auto& [_, food]: this->world->food) {
                this->window->draw(graphics::FoodDrawer::Get(&food));
            }
            this->window->display();

            time_diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
            if (time_diff < frame_micro_sec) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(frame_micro_sec - time_diff));
            }
        }
    }

    App::App() {
        // todo provide configuration?
        this->window = new sf::RenderWindow(sf::VideoMode(2000, 1500), "Cells");
        window->setVerticalSyncEnabled(true);

        this->world = new core::World();
        this->logic = new logic::Logic(*this->world);
    }

    App::~App() {
        delete this->window;
        delete this->logic;
        delete this->world;
    }

    namespace core {
        static Position GenerateRandomPosition(
                int width,
                int height,
                const std::vector<Position> &occupied_positions,
                int min_distance_between_cells
        ) {
            float x, y, min_dist_between_cells = static_cast<float>(min_distance_between_cells);
            bool is_ok = false;
            while (not is_ok) {
                is_ok = true;
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                std::uniform_int_distribution<int> x_distribution(0, width);
                std::uniform_int_distribution<int> y_distribution(0, height);
                x = static_cast<float>(x_distribution(generator));
                y = static_cast<float>(y_distribution(generator));
                for (auto& p: occupied_positions) {
                    if (abs(p.X() - x) <= min_dist_between_cells && abs(p.Y() - y) <= min_dist_between_cells) {
                        is_ok = false;
                        break;
                    }
                }
            }
            return {x, y};
        }

        std::vector<Position> GenerateRandomPositions(
                int field_width,
                int field_height,
                int size,
                int min_distance_between_cells
        ) {
            std::vector<Position> positions;
            positions.reserve(size);
            for (int i = 0; i < size; i++) {
                auto position = GenerateRandomPosition(
                        field_width, field_height, positions, min_distance_between_cells
                );
                positions.push_back(position);
            }
            return positions;
        }
    }
}
