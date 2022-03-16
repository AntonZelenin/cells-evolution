#include "CellsEvo/core.h"
#include <random>


namespace cells_evo::food {
    std::vector<Food> Manager::GetFirstGeneration(
            int field_width,
            int field_height,
            int size = Manager::kFirstGenerationSize
    ) const {
        std::vector<Food> food;
        food.reserve(size);
        auto positions = cells_evo::GenerateRandomPositions(
                field_width, field_height, size, this->kMinDistanceBetweenItems
        );
        for (auto position: positions) {
            food.emplace_back(position);
        }
        return food;
    }

    Food::Food(Position position) {
        this->position = position;
    }
}
