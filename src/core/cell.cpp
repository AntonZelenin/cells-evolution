#include "CellsEvo/core.h"

namespace cells_evo::cell {
    Cell::Cell(float size, float speed, Type type, Position position) {
        this->size = size;
        this->speed = speed;
        this->type = type;
        this->position = position;
    }

    std::vector<Cell> Manager::GetFirstGeneration(
            int field_width,
            int field_height,
            int size = Manager::kFirstGenerationSize
    ) const {
        std::vector<Cell> cells;
        cells.reserve(size);
        auto positions = cells_evo::GenerateRandomPositions(
                field_width, field_height, size, this->kMinDistanceBetweenCells
        );
        for (auto position: positions) {
            cells.emplace_back(
                    this->kDefaultCellSize,
                    this->kDefaultCellSpeed,
                    Manager::GetCellType(),
                    position
            );
        }
        return cells;
    }

    Type Manager::GetCellType() {
        return Type::kHunter;
    }
}
