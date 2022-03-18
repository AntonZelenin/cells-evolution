#include <random>
#include "CellsEvo/core.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
    void App::Run() {
        auto world = core::World();
        while (this->window->isOpen()) {
            sf::Event event{};
            while (this->window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    this->window->close();
            }
            this->window->clear(sf::Color::Black);
            for (auto cell: world.cells) {
                this->window->draw(CellDrawer().Get(&cell));
            }
            for (auto f: world.food) {
                this->window->draw(FoodDrawer::Get(&f));
            }
            this->window->display();
        }
    }

    App::App() {
        // todo provide configuration?
        this->window = new sf::RenderWindow(sf::VideoMode(2000, 1500), "Cells");
        window->setVerticalSyncEnabled(true);
    }

    App::~App() {
        delete this->window;
    }

    namespace cells_evo::core {
        // todo why do I need to specify namespace when I'm already in it?
        static ::cells_evo::core::Position GenerateRandomPosition(
                int width,
                int height,
                const std::vector<::cells_evo::core::Position> &occupied_positions,
                int min_distance_between_cells
        ) {
            int x, y;
            bool is_ok = false;
            while (not is_ok) {
                is_ok = true;
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                std::uniform_int_distribution<int> x_distribution(0, width);
                std::uniform_int_distribution<int> y_distribution(0, height);
                x = x_distribution(generator);
                y = y_distribution(generator);
                for (auto p: occupied_positions) {
                    if (abs(p.x - x) <= min_distance_between_cells && abs(p.y - y) <= min_distance_between_cells) {
                        is_ok = false;
                        break;
                    }
                }
            }
            return {x, y};
        }

        std::vector<::cells_evo::core::Position> GenerateRandomPositions(
                int field_width,
                int field_height,
                int size,
                int min_distance_between_cells
        ) {
            std::vector<::cells_evo::core::Position> positions;
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
