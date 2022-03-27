#include <cmath>

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

    Position Position::operator-(const Position &pos) const {
        return {pos.coordinates - this->coordinates};
    }

    Position::Position(Vector2<float> coords) {
        this->coordinates = coords;
    }

    template<typename T>
    Vector2<T>::Vector2(T x, T y) {
        this->x = x;
        this->y = y;
    }

    template<typename T>
    float Vector2<T>::Magnitude() {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

    template<typename T>
    void Vector2<T>::Normalize() {
        auto len = this->Magnitude();
        if (len == 0)
            return;
        this->x /= len;
        this->y /= len;
    }

    template<typename T>
    Vector2<float> Vector2<T>::operator-(const Vector2<float> &obj) const {
        return Vector2<float>{obj.x - this->x, obj.y - this->y};
    }

    // todo
    Vector2<float> GetDirectionVector(Position &origin, Position &destination) {
        Vector2<float> vec{destination.X() - origin.X(), destination.Y() - origin.Y()};
        vec.Normalize();
        return vec;
    }
}
