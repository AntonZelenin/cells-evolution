#include "CellsEvo/Core/Drawer.h"

Cell::Cell() {
    this->size = 20.f;
    this->speed = 1.f;
    this->type = CellType::nonhunter;
    this->position = Position(100, 100);
}
