#ifndef CELLS_EVOLUTION_FOOD_H
#define CELLS_EVOLUTION_FOOD_H

namespace cells_evo::core {
    class Food: core::HasPosition {
    public:
        Position position;

        explicit Food(Position position);
        Position& GetPosition() override;
    };

    class FoodGenerator {
        static const int kFirstGenerationSize = 20;
        const int kMinDistanceBetweenItems = 1;

    public:
        [[nodiscard]] std::vector<Food> GetFirstGeneration(int field_width, int field_height, int size) const;
    };
}

#endif //CELLS_EVOLUTION_FOOD_H
