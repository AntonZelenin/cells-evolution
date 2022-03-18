#ifndef CELLS_EVOLUTION_BASE_H
#define CELLS_EVOLUTION_BASE_H

#include "geometry.h"

namespace cells_evo::core {
    class HasPosition {
    public:
        virtual Position& GetPosition() = 0;
    };
}

#endif //CELLS_EVOLUTION_BASE_H
