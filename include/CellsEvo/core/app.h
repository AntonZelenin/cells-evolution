#ifndef CELLS_EVOLUTION_APP_H
#define CELLS_EVOLUTION_APP_H


#include "SFML/Graphics/RenderWindow.hpp"
#include "cell.h"
#include "food.h"

namespace cells_evo {
    class App {
        sf::RenderWindow *window;
        cell::Manager cell_manager{};
        food::Manager food_manager{};
    public:
        App();

        ~App();

        void Run();
    };

    // todo find it a better place
    [[nodiscard]] Position GeneratePosition(
            int width,
            int height,
            const std::vector<Position> &occupied_positions,
            int min_distance_between_cells
    );
}

#endif //CELLS_EVOLUTION_APP_H
