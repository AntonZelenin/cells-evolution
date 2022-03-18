#ifndef CELLS_EVOLUTION_GEOMETRY_H
#define CELLS_EVOLUTION_GEOMETRY_H

namespace cells_evo::core {
    template<typename T>
    struct Vector2 {
        T x;
        T y;

        Vector2() = default;

        Vector2(T x, T y);
    };

    struct Position {
        Vector2<float> coordinates{};

        // todo another construct implementation?
        Position(float x, float y);

        [[nodiscard]] float X() const;
        [[nodiscard]] float Y() const;
    };

    Vector2<float> GetDirectionVector(Position& origin, Position& destination);
}

#endif //CELLS_EVOLUTION_GEOMETRY_H
