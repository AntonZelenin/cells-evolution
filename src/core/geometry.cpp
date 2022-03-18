#include "CellsEvo/core/geometry.h"

namespace cells_evo::core {
    Position::Position(float x, float y) {
        this->coordinates = Vector2<float>(x, y);
    }

    float Position::X() const {
        return this->coordinates.x;
    }

    float Position::Y() const {
        return this->coordinates.y;
    }

    template<typename T>
    Vector2<T>::Vector2(T x, T y) {
        this->x = x;
        this->y = y;
    }

    // todo
    Vector2<float> GetDirectionVector(Position& origin, Position& destination) {
        return {destination.X() - origin.X(), destination.Y() - origin.Y()};
    }
}
