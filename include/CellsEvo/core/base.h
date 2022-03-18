#ifndef CELLS_EVOLUTION_BASE_H
#define CELLS_EVOLUTION_BASE_H

#include "geometry.h"

namespace cells_evo::core {
    class Entity {
    public:
        virtual Position& GetPosition() = 0;
        virtual unsigned int GetId() = 0;
    };
}

#endif //CELLS_EVOLUTION_BASE_H
