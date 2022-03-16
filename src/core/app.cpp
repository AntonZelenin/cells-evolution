#include <random>
#include "CellsEvo/core.h"
#include "CellsEvo/graphics.h"
#include "SFML/Graphics.hpp"

namespace cells_evo {
    void App::Run() {
        auto cells = this->cell_manager.GetFirstGeneration(
                static_cast<int>(this->window->getSize().x),
                static_cast<int>(this->window->getSize().y),
                1
        );
        // todo I cannot use default args?
        auto food = this->food_manager.GetFirstGeneration(
                static_cast<int>(this->window->getSize().x),
                static_cast<int>(this->window->getSize().y),
                20
        );
        while (this->window->isOpen()) {
            sf::Event event{};
            while (this->window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    this->window->close();
            }
            this->window->clear(sf::Color::Black);
            for (auto cell: cells) {
                this->window->draw(CellDrawer().Get(&cell));
            }
            for (auto f: food) {
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

    Position GeneratePosition(
            int width,
            int height,
            const std::vector<Position> &occupied_positions,
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
}
