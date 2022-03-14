#include "CellsEvo/Graphics/Drawer.h"


float CellDrawer::get_thickness(int size) const {
    return static_cast<float>(size) / this->THICKNESS_COEFFICIENT;
}

CellShape CellDrawer::get(const Cell *cell) {
    CellShape shape(static_cast<float>(cell->size));
    shape.setOutlineThickness(this->get_thickness(cell->size));
    shape.setOutlineColor(this->color_provider.get(&cell->type));
    return shape;
}

sf::Color CellColorProvider::get(const CellType *type) {
    return this->mapping[*type];
}
