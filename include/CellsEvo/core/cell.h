#ifndef CELLS_EVOLUTION_CELL_H
#define CELLS_EVOLUTION_CELL_H

#include <vector>
#include "geometry.h"

namespace cells_evo::cell {
    enum Type {
        kHunter, kNonhunter
    };

    struct Cell {
        float size;
        float speed;
        Type type;

        Position position;

        Cell(float size, float speed, Type type, Position position);
    };

    class Manager {
        static const int kFirstGenerationSize = 20;
        const float kDefaultCellSize = 20.0;
        const float kDefaultCellSpeed = 1.0;
        const int kMinDistanceBetweenCells = 1;

        static Type GetCellType();
    public:
        [[nodiscard]] std::vector<Cell> GetFirstGeneration(int field_width, int field_height, int size) const;
    };
}

#endif //CELLS_EVOLUTION_CELL_H
