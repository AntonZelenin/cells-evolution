#ifndef CELLS_EVOLUTION_LOGIC_H
#define CELLS_EVOLUTION_LOGIC_H

#include "cell.h"
#include "world.h"

namespace cells_evo::logic {
    class Logic {
        core::World &world;

        void CellAct(core::Cell &cell);

        core::Food &GetClosestFood();

    public:
        explicit Logic(core::World &world);

        void WorldTick();
    };
}

#endif //CELLS_EVOLUTION_LOGIC_H
