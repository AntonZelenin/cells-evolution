#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/core.h"

class CellColorProvider {
    std::map<cell::Type, sf::Color> mapping{
            {cell::Type::kHunter,    sf::Color::Red},
            {cell::Type::kNonhunter, sf::Color::Green},
    };
public:
    sf::Color Get(const cell::Type* type);
};

class CellDrawer {
    const float k_thickness_coefficient = 5.f;

    CellColorProvider color_provider = CellColorProvider();

    [[nodiscard]] float GetThickness(float size) const;
public:
    sf::CircleShape Get(const cell::Cell* cell);
};

// todo make interface for drawers
class FoodDrawer {
public:
    static sf::RectangleShape Get(const food::Food *food);
};

#endif //CELLS_EVOLUTION_DRAWER_H
