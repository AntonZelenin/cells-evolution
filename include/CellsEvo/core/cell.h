#ifndef CELLS_EVOLUTION_CELL_H
#define CELLS_EVOLUTION_CELL_H

#include <vector>
#include "geometry.h"
#include "genes.h"
#include "base.h"

namespace cells_evo::core {
    enum Type {
        kHunter, kNonhunter
    };

    class Cell: public core::Entity {
        unsigned int id;
        Position position;
    public:
        float size;
        float speed;
        Type type;
        std::vector<Gene> genes{};

        Cell(float size, float speed, Type type, Position position);
        Position& GetPosition() override;
        void SetPosition(Position pos) override;
        unsigned int GetId() override;
        void SetId(unsigned int id);
    };

    class CellGenerator {
        static const int kFirstGenerationSize = 20;
        const float kDefaultCellSize = 20.0;
        const float kDefaultCellSpeed = 1.0;
        const int kMinDistanceBetweenCells = 1;

        static Type GetCellType();
    public:
        [[nodiscard]] std::vector<Cell> GetFirstGeneration(int field_width, int field_height, int generation_size) const;
    };
}

#endif //CELLS_EVOLUTION_CELL_H
