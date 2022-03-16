#include "CellsEvo/core.h"
#include <random>


namespace food {
// todo this is duplicate, refactor it
    std::vector<Food> Manager::GetFirstGeneration(
            int field_width,
            int field_height,
            int size = Manager::kFirstGenerationSize
    ) {
        std::vector<Food> food;
        food.reserve(size);
        std::vector<Position> positions;
        positions.reserve(size);
        for (int i = 0; i < size; i++) {
            auto position = cells_evo::GeneratePosition(
                    field_width, field_height, positions, this->kMinDistanceBetweenItems
            );
            positions.push_back(position);
            food.emplace_back(position);
        }
        return food;
    }

    Food::Food(Position position) {
        this->position = position;
    }
}
