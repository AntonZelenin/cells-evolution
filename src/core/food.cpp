#include "CellsEvo/core.h"
#include "CellsEvo/core/food.h"

#include <random>


namespace cells_evo::core {
    std::vector<Food> FoodGenerator::GetFirstGeneration(
            int field_width,
            int field_height,
            int size = FoodGenerator::kFirstGenerationSize
    ) const {
        std::vector<Food> food;
        food.reserve(size);
        auto positions = cells_evo::core::GenerateRandomPositions(
                field_width, field_height, size, this->kMinDistanceBetweenItems
        );
        for (auto& position: positions) {
            food.emplace_back(position);
        }
        return food;
    }

    Food::Food(Position position) : position(position) {
        this->id = 0;
    }

    Position& Food::GetPosition() {
        return this->position;
    }

    unsigned int Food::GetId() {
        return this->id;
    }

    void Food::SetId(unsigned int id) {
        this->id = id;
    }

    void Food::SetPosition(Position pos) {
        this->position = pos;
    }
}
