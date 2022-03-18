#include "CellsEvo/core.h"
#include "CellsEvo/core/cell.h"


namespace cells_evo::core {
    Cell::Cell(float size, float speed, Type type, Position position) {
        this->id = 0;
        this->size = size;
        this->speed = speed;
        this->type = type;
        this->position = position;
    }

    Position& Cell::GetPosition() {
        return this->position;
    }

    unsigned int Cell::GetId() {
        // todo duplicate
        return this->id;
    }

    void Cell::SetId(unsigned int id) {
        // todo duplicate
        this->id = id;
    }

    std::vector<Cell> CellGenerator::GetFirstGeneration(
            int field_width,
            int field_height,
            int size = CellGenerator::kFirstGenerationSize
    ) const {
        std::vector<Cell> cells;
        cells.reserve(size);
        auto positions = cells_evo::core::GenerateRandomPositions(
                field_width, field_height, size, this->kMinDistanceBetweenCells
        );
        for (auto position: positions) {
            cells.emplace_back(
                    this->kDefaultCellSize,
                    this->kDefaultCellSpeed,
                    CellGenerator::GetCellType(),
                    position
            );
        }
        return cells;
    }

    Type CellGenerator::GetCellType() {
        return Type::kHunter;
    }
}
