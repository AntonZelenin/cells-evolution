#ifndef CELLS_EVOLUTION_WORLD_H
#define CELLS_EVOLUTION_WORLD_H

#include <vector>
#include "CellsEvo/core/cell.h"
#include "CellsEvo/core/food.h"

namespace cells_evo::core {
    class World {
    public:
        int width;
        int height;

        std::vector<Food> food;
        std::vector<Cell> cells;

        World();
    };

    core::HasPosition* FindNearest(Position position, std::vector<core::HasPosition>);
}

#endif //CELLS_EVOLUTION_WORLD_H
