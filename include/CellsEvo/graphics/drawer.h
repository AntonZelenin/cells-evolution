#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/core.h"

namespace cells_evo::graphics {
    class CellColorProvider {
        std::map<core::Type, sf::Color> mapping_{
                {core::Type::K_HUNTER, sf::Color::Red},
                {core::Type::K_NONHUNTER, sf::Color::Green},
        };
    public:
        sf::Color Get(const core::Type *type);
    };

    class CellDrawer {
        const float k_thickness_coefficient_ = 4.f;

        CellColorProvider color_provider_ = CellColorProvider();

        [[nodiscard]] float GetThickness(float size) const;

    public:
        sf::CircleShape Get(core::Cell *cell);
    };

// todo make interface for drawers
    class FoodDrawer {
    public:
        static sf::RectangleShape Get(core::Food *food);
    };
}

#endif //CELLS_EVOLUTION_DRAWER_H
