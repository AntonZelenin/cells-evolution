#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/core.h"

class CellColorProvider {
    std::map<cells_evo::core::Type, sf::Color> mapping{
            {cells_evo::core::Type::kHunter,    sf::Color::Red},
            {cells_evo::core::Type::kNonhunter, sf::Color::Green},
    };
public:
    sf::Color Get(const cells_evo::core::Type* type);
};

class CellDrawer {
    const float k_thickness_coefficient = 5.f;

    CellColorProvider color_provider = CellColorProvider();

    [[nodiscard]] float GetThickness(float size) const;
public:
    sf::CircleShape Get(cells_evo::core::Cell *cell);
};

// todo make interface for drawers
class FoodDrawer {
public:
    static sf::RectangleShape Get(cells_evo::core::Food *food);
};

#endif //CELLS_EVOLUTION_DRAWER_H
