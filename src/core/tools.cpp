#include "CellsEvo/core/tools.h"

namespace cells_evo::core {
uint IndexDriver::GetNextId() {
  return idx_++;
}

Position RandomPositionsGenerator::GenerateRandomPosition(
    int width,
    int height,
    const std::vector<Position> &occupied_positions,
    int min_distance_between_cells
) {
//  auto min_dist_between_cells = static_cast<float>(min_distance_between_cells);
  Vector2<float> pos{};
  bool is_ok = false;

  while (not is_ok) {
    is_ok = true;
    pos = Vector2<float>(
        random_engine_.GetUniformReal(0, static_cast<float>(width)),
        random_engine_.GetUniformReal(0, static_cast<float>(height))
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

Position RandomPositionsGenerator::GenerateRandomPositionInRadius(Position &center, float radius) {
  return {
      random_engine_.GetUniformReal(center.x - radius, center.x + radius),
      random_engine_.GetUniformReal(center.y - radius, center.y + radius)
  };
}

std::vector<Position> RandomPositionsGenerator::GenerateRandomPositions(
    int field_width,
    int field_height,
    int number_of_positions,
    int min_distance_between_positions
) {
  std::vector<Position> positions;
  positions.reserve(number_of_positions);
  for (int i = 0; i < number_of_positions; i++) {
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

std::vector<Position> RandomPositionsGenerator::GenerateRandomPositionsInRadius(
    Position center,
    float radius,
    int field_width,
    int field_height,
    int number_of_positions
) {
  std::vector<Position> positions;
  positions.reserve(number_of_positions);
  for (int i = 0; i < number_of_positions;) {
    auto position = GenerateRandomPositionInRadius(center, radius);
    if (IsSuitablePosition(position, field_width, field_height)) {
      positions.push_back(position);
      i++;
    }
  }
  return positions;
}

bool RandomPositionsGenerator::IsSuitablePosition(Position position, float max_x, float max_y) {
  return position.x >= 0 && position.x <= max_x && position.y >= 0.0 && position.y <= max_y;
}
}
