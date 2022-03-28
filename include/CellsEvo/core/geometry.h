#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_

namespace cells_evo::core {
template<typename T>
struct Vector2 {
  T x;
  T y;

  Vector2() = default;

  Vector2(T x, T y);

  float Magnitude();

  void Normalize();

  Vector2<float> operator-(Vector2<float> const &obj) const;
};

struct Position {
  Vector2<float> coordinates{};

  Position() = default;

  Position(float x, float y);

  explicit Position(Vector2<float> coords);

  [[nodiscard]] float X() const;

  [[nodiscard]] float Y() const;

  Vector2<float> operator-(Position const &pos) const;
};

Vector2<float> GetDirectionVector(Position &origin, Position &destination);
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_GEOMETRY_H_
