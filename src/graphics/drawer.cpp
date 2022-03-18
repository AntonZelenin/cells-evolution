#include "CellsEvo/graphics.h"
#include "CellsEvo/core.h"

namespace cells_evo::graphics {
    float CellDrawer::GetThickness(float size) const {
        return size / this->k_thickness_coefficient;
    }

    sf::CircleShape CellDrawer::Get(core::Cell *cell) {
        // todo make core attrs private float? and add public methods to manipulate? or another class to Get rid of casts
        sf::CircleShape shape(static_cast<float>(cell->size));
        shape.setOutlineThickness(this->GetThickness(cell->size));
        shape.setOutlineColor(this->color_provider.Get(&cell->type));
        shape.setPosition(static_cast<float>(cell->GetPosition().X()), static_cast<float>(cell->GetPosition().Y()));
        return shape;
    }

    sf::Color CellColorProvider::Get(const core::Type *type) {
        return this->mapping[*type];
    }

    sf::RectangleShape FoodDrawer::Get(core::Food *food) {
        sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
        shape.setFillColor(sf::Color::Cyan);
        shape.setPosition(static_cast<float>(food->GetPosition().X()), static_cast<float>(food->GetPosition().Y()));
        return shape;
    }
}
