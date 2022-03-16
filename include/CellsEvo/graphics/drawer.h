#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/core.h"

class CellColorProvider {
    std::map<cells_evo::cell::Type, sf::Color> mapping{
            {cells_evo::cell::Type::kHunter,    sf::Color::Red},
            {cells_evo::cell::Type::kNonhunter, sf::Color::Green},
    };
public:
    sf::Color Get(const cells_evo::cell::Type* type);
};

class CellDrawer {
    const float k_thickness_coefficient = 5.f;

    CellColorProvider color_provider = CellColorProvider();

    [[nodiscard]] float GetThickness(float size) const;
public:
    sf::CircleShape Get(const cells_evo::cell::Cell* cell);
};

// todo make interface for drawers
class FoodDrawer {
public:
    static sf::RectangleShape Get(const cells_evo::food::Food *food);
};

#endif //CELLS_EVOLUTION_DRAWER_H
