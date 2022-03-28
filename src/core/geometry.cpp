#include <cmath>

#include "CellsEvo/core/geometry.h"

namespace cells_evo::core {
Position::Position(float x, float y) {
  coordinates = Vector2<float>(x, y);
}

float Position::X() const {
  return coordinates.x;
}

float Position::Y() const {
  return coordinates.y;
}

Vector2<float> Position::operator-(const Position &pos) const {
  return {pos.coordinates - coordinates};
}

Position::Position(Vector2<float> coords) {
  coordinates = coords;
}

template<typename T>
Vector2<T>::Vector2(T x, T y) {
  x = x;
  y = y;
}

template<typename T>
float Vector2<T>::Magnitude() {
  return std::sqrt(x * x + y * y);
}

template<typename T>
void Vector2<T>::Normalize() {
  auto len = Magnitude();
  if (len == 0)
    return;
  x /= len;
  y /= len;
}

template<typename T>
Vector2<float> Vector2<T>::operator-(const Vector2<float> &obj) const {
  return Vector2<float>{obj.x - x, obj.y - y};
}

// todo
Vector2<float> GetDirectionVector(Position &origin, Position &destination) {
  Vector2<float> vec{destination.X() - origin.X(), destination.Y() - origin.Y()};
  vec.Normalize();
  return vec;
}
}
