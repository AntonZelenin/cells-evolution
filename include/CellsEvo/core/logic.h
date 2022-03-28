#ifndef CELLS_EVOLUTION_LOGIC_H
#define CELLS_EVOLUTION_LOGIC_H

#include "cell.h"
#include "world.h"

namespace cells_evo::logic {
    class Logic {
        core::World &world_;
        std::map<unsigned int, unsigned int> cell_food_cache_;

        void CellAct(core::Cell &cell);

        core::Food &FindClosestFood(core::Cell &cell);

        void MoveCells();

        void ProcessEvents();

        void BuildCellsFoodCache();

        void ClearCache();

        static bool CellGotFood(core::Cell &cell, core::Food &food);

        void RemoveFoodFromCache(const core::Food &food);

        void ProcessEatFood(core::Cell &cell);

        void GenerateFood();

        void DivideCells();

    public:
        explicit Logic(core::World &world);

        void WorldTick();
    };
}

#endif //CELLS_EVOLUTION_LOGIC_H
