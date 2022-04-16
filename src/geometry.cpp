#include <cmath>

#include "CellsEvo/geometry.h"

namespace cells_evo::core {
template<typename T>
Vector2<T>::Vector2(T x, T y) {
  this->x = x;
  this->y = y;
}

template<typename T>
float Vector2<T>::Magnitude() {
//  if (!cached_sqrt_)
//    cached_sqrt_ = std::sqrt(MagnitudeSquared());
//  return cached_sqrt_.value();
  return std::sqrt(MagnitudeSquared());
}

template<typename T>
float Vector2<T>::MagnitudeSquared() {
  return x * x + y * y;
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

//template<typename T>
//Vector2<float> Vector2<T>::operator+(const Vector2<float> &obj) const {
//  return Vector2<float>{obj.x + x, obj.y + y};
//}

// todo strange definition -_-
template<>
void Vector2<float>::operator+=(const Vector2<float> &obj) {
  x += obj.x;
  y += obj.y;
}

template<>
Vector2<float> Vector2<float>::operator*(float val) const {
  return Vector2<float>{x * val, y * val};
}

Vector2<float> GetDirectionVector(Position &vec_1, Position &vec_2) {
  Vector2<float> vec{vec_1 - vec_2};
  vec.Normalize();
  return vec;
}
}
