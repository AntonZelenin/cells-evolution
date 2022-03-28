#ifndef CELLS_EVOLUTION_FOOD_H
#define CELLS_EVOLUTION_FOOD_H

namespace cells_evo::core {
    class Food : Entity {
        Position position_;
    public:
        int id_;

        explicit Food(Position position);

        Position &GetPosition() override;

        void SetPosition(Position) override;

        unsigned int GetId() override;

        void SetId(unsigned int id);
    };

    class FoodGenerator {
        static const int k_first_generation_size_ = 20;
        const int k_min_distance_between_items_ = 1;

    public:
        [[nodiscard]] std::vector<Food> Generate(int field_width, int field_height, int size) const;
    };
}

#endif //CELLS_EVOLUTION_FOOD_H
