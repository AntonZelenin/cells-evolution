#ifndef CELLS_EVOLUTION_FOOD_H
#define CELLS_EVOLUTION_FOOD_H


#include "cell.h"

namespace cells_evo::food {
    class Food {
    public:
        Position position;

        explicit Food(Position position);
    };

    class Manager {
        static const int kFirstGenerationSize = 20;
        const int kMinDistanceBetweenItems = 1;

    public:
        [[nodiscard]] std::vector<Food> GetFirstGeneration(int field_width, int field_height, int size) const;
    };
}

#endif //CELLS_EVOLUTION_FOOD_H
