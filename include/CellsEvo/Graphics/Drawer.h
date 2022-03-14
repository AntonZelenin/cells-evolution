#ifndef CELLS_EVOLUTION_DRAWER_H
#define CELLS_EVOLUTION_DRAWER_H

#include <map>
#include <SFML/Graphics.hpp>
#include "CellsEvo/Core/Drawer.h"

class CellShape : public sf::CircleShape {
    using CircleShape::CircleShape;
};

class CellColorProvider {
    std::map<CellType, sf::Color> mapping{
            {CellType::hunter, sf::Color::Red},
            {CellType::nonhunter, sf::Color::Green},
    };
public:
    sf::Color get(const CellType* type);
};

class CellDrawer {
    float THICKNESS_COEFFICIENT = 5.f;

    CellColorProvider color_provider = CellColorProvider();

    [[nodiscard]] float get_thickness(int size) const;
public:
    CellShape get(const Cell* cell);
};

#endif //CELLS_EVOLUTION_DRAWER_H
