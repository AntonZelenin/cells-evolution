#include "CellsEvo/core/tools.h"


namespace cells_evo::core {
unsigned int IndexDriver::GetNextId() {
  return idx_++;
}

std::vector<int> GetRandomInts(int from, int  to, unsigned int number) {
  std::vector<int> numbers{};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(from, to);

  for (; number > 0; number--) {
    numbers.push_back(distribution(generator));
  }

  return numbers;
}

std::vector<float> GetRandomFloats(float from, float  to, unsigned int number) {
  std::vector<float> numbers{};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution(from, to);

  for (; number > 0; number--) {
    numbers.push_back(distribution(generator));
  }

  return numbers;
}

// todo they are very similar -_-
std::vector<float> GetRandomNormalFloats(float from, float  to, unsigned int number) {
  std::vector<float> numbers{};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::normal_distribution<float> distribution(from, to);

  for (; number > 0; number--) {
    numbers.push_back(distribution(generator));
  }

  return numbers;
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
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> x_distribution(0, width);
  std::uniform_int_distribution<int> y_distribution(0, height);
  while (not is_ok) {
    is_ok = true;
    pos = Vector2<float>(
        static_cast<float>(x_distribution(generator)),
        static_cast<float>( y_distribution(generator))
    );
    for (auto &p : occupied_positions) {
      if ((p.coordinates - pos).Magnitude() <= min_dist_between_cells) {
        is_ok = false;
        break;
      }
    }
  }
  return Position(pos);
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
