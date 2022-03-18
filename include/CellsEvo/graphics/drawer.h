#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/core.h"

namespace cells_evo::graphics {
    class CellColorProvider {
        std::map<core::Type, sf::Color> mapping{
                {core::Type::kHunter,    sf::Color::Red},
                {core::Type::kNonhunter, sf::Color::Green},
        };
    public:
        sf::Color Get(const core::Type *type);
    };

    class CellDrawer {
        const float k_thickness_coefficient = 5.f;

        CellColorProvider color_provider = CellColorProvider();

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
