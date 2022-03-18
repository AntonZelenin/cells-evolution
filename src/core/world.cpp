#include "CellsEvo/core/world.h"

namespace cells_evo::core {
    World::World() {
        this->width = 1920;
        this->height = 1080;
        this->food = FoodGenerator().GetFirstGeneration(
                this->width,
                this->height,
                20
        );
        this->cells = CellGenerator().GetFirstGeneration(
                this->width,
                this->height,
                5
        );
    }
}
