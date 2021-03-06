#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_

#include <optional>

namespace cells_evo::core {
template<typename T>
struct Vector2;
typedef Vector2<float> Position;

template<typename T>
struct Vector2 {
 private:
  std::optional<float> cached_sqrt_ = {};

 public:
  T x;
  T y;

  Vector2() = default;
  Vector2(T x, T y);
  float Magnitude();
  float MagnitudeSquared();
  void Normalize();

  // todo not generic definitions -_-
  Vector2<float> operator-(Vector2<float> const &obj) const;
//  Vector2<float> operator+(Vector2<float> const &obj) const;
  void operator+=(Vector2<float> const &obj);
  Vector2<float> operator*(float val) const;
};

Vector2<float> GetDirectionVector(Position &origin, Position &destination);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_
