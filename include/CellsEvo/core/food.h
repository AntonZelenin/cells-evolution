#ifndef CELLS_EVOLUTION_FOOD_H
#define CELLS_EVOLUTION_FOOD_H

namespace cells_evo::core {
    class Food : Entity {
        int id;
        Position position;
    public:

        explicit Food(Position position);

        Position &GetPosition() override;

        void SetPosition(Position) override;

        unsigned int GetId() override;

        void SetId(unsigned int id);
    };

    class FoodGenerator {
        static const int kFirstGenerationSize = 20;
        const int kMinDistanceBetweenItems = 1;

    public:
        [[nodiscard]] std::vector<Food> GetFirstGeneration(int field_width, int field_height, int size) const;
    };
}

#endif //CELLS_EVOLUTION_FOOD_H
