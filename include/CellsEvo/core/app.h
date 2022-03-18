#ifndef CELLS_EVOLUTION_APP_H
#define CELLS_EVOLUTION_APP_H


#include "SFML/Graphics/RenderWindow.hpp"
#include "cell.h"
#include "food.h"

namespace cells_evo {
    class App {
        sf::RenderWindow *window;
    public:
        App();
        ~App();

        void Run();
    };

    namespace cells_evo::core {
        // todo find it a better place?
        std::vector<::cells_evo::core::Position> GenerateRandomPositions(
                int field_width,
                int field_height,
                int size,
                int min_distance_between_cells
        );
    }
}

#endif //CELLS_EVOLUTION_APP_H
