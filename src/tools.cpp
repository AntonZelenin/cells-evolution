#include "CellsEvo/tools.h"

namespace cells_evo::core {
uint IndexDriver::GetNextId() {
  return idx_++;
}

static Position GenerateRandomPosition(
    int width,
    int height,
    const std::vector<Position> &occupied_positions,
    int min_distance_between_cells
) {
  auto min_dist_between_cells = static_cast<float>(min_distance_between_cells);
  Vector2<float> pos{};
  bool is_ok = false;

  std::random_device dev;
  std::mt19937 generator(dev());
  std::uniform_int_distribution<int> x_distribution(0, width);
  std::uniform_int_distribution<int> y_distribution(0, height);

  while (not is_ok) {
    is_ok = true;
    pos = Vector2<float>(
        static_cast<float>(x_distribution(generator)),
        static_cast<float>( y_distribution(generator))
    );
//    for (auto &p : occupied_positions) {
//      if ((p - pos).Magnitude() <= min_dist_between_cells) {
//        is_ok = false;
//        break;
//      }
//    }
  }
  return pos;
}

std::vector<Position> GenerateRandomPositions(
    int field_width,
    int field_height,
    int size,
    int min_distance_between_positions
) {
  std::vector<Position> positions;
  positions.reserve(size);
  for (int i = 0; i < size; i++) {
    auto position = GenerateRandomPosition(
        field_width,
        field_height,
        positions,
        min_distance_between_positions
    );
    positions.push_back(position);
  }
  return positions;
}
}
