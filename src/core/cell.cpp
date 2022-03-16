#include "CellsEvo/core.h"

namespace cell {
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
    ) {
        std::vector<Cell> cells;
        cells.reserve(size);
        std::vector<Position> positions;
        positions.reserve(size);
        for (int i = 0; i < size; i++) {
            auto position = cells_evo::GeneratePosition(
                    field_width, field_height, positions, this->kMinDistanceBetweenCells
            );
            positions.push_back(position);
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
