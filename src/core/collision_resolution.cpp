#include "CellsEvo/core/collision_resolution.h"

namespace cells_evo::collisions {
static bool CellsCollide(core::Cell &cell_1, core::Cell &cell_2) {
  return (cell_1.GetPosition() - cell_2.GetPosition()).MagnitudeSquared()
      < pow((cell_1.GetSize() + cell_2.GetSize()), 2);
}

static bool CellFoodCollide(core::Cell &cell, core::Food &food) {
  return (cell.GetPosition() - food.GetPosition()).MagnitudeSquared()
      < pow((cell.GetSize() + food.GetSize()), 2);
}

IdXPairs CollisionDetector::DetectCellCollisions(core::CellStorage &cells) {
  // todo it assumes input is sorted
  if (cells.size() < 2)
    return {};

  IdXPairs colliding_cell_indices;
  // todo a cell can collide with more than one
  for (auto cell_idx = 1; cell_idx < cells.size(); ++cell_idx) {
    auto &cell_1 = cells.at(cell_idx), &cell_2 = cells.at(cell_idx - 1);
    if (cell_1.IsDeleted() || cell_2.IsDeleted()) continue;
    if (CellsCollide(cell_1, cell_2)) {
      colliding_cell_indices.push_back({cell_idx, cell_idx - 1});
    }
  }

  return colliding_cell_indices;
}

FoodCellCollisions CollisionDetector::DetectCellFoodCollisions(core::CellStorage &cells, core::FoodStorage &foods) {
  // todo cells change direction when new but far food appears
  if (cells.empty() || foods.empty())
    return {};

  auto comparison = [](const core::Food &a, const core::Food &b) {
    return a.GetSize() < b.GetSize();
  };
  auto max_size_food = std::max_element(foods.begin(), foods.end(), comparison);

  FoodCellCollisions colliding;
  for (uint food_idx = 0, cell_idx = 0; food_idx < foods.size() && cell_idx < cells.size();) {
     auto &cell = cells[cell_idx];
    auto &food = foods[food_idx];

    if (cell.IsDeleted()) {
      ++cell_idx;
      continue;
    }
    if (food.IsDeleted()) {
      ++food_idx;
      continue;
    }

    auto x_diff = cell.GetPosition().x - food.GetPosition().x;
    auto abs_x_diff = abs(x_diff);
    if (abs_x_diff > cell.GetSize() + food.GetSize()) {
//      if (x_diff > 0 || abs_x_diff < max_size_food->GetSize()) {
      if (x_diff > 0) {
        // if it's to the left of the food
        ++food_idx;
        continue;
      } else {
        // if it's to the right of the food
        ++cell_idx;
        continue;
      }
    } else {
      if ((cell.GetPosition() - food.GetPosition()).MagnitudeSquared() <= pow(cell.GetSize() + food.GetSize(), 2)) {
        colliding.push_back(FoodCellCollision{food_idx, cell_idx});
        ++cell_idx;
      } else {
        if (x_diff > 0) {
          // if it's to the left of the food
          ++food_idx;
          continue;
        } else {
          // if it's to the right of the food
          ++cell_idx;
          continue;
        }
      }
    }
  }

  return colliding;
}

//uint BinarySearchClosestFoodX(
//    core::FoodStorage &foods,
//    int l,
//    int r,
//    float needle,
//    float min_distance,
//    uint closest_idx
//) {
//  if (r >= l) {
//    int mid = l + (r - l) / 2;
//
////    if (foods.at(mid).IsDeleted()) {
//    // todo what to do?
////    }
//
//    auto curr_x = foods.at(mid).GetPosition().x;
//    auto dist = abs(needle - curr_x);
//    if (curr_x == needle)
//      return mid;
//    if (dist < min_distance) {
//      min_distance = dist;
//      closest_idx = mid;
//    }
//
//    if (curr_x > needle)
//      return BinarySearchClosestFoodX(foods, l, mid - 1, needle, min_distance, closest_idx);
//
//    return BinarySearchClosestFoodX(foods, mid + 1, r, needle, min_distance, closest_idx);
//  }
//
//  return closest_idx;
//}
//
//// todo move
//std::optional<uint> CollisionDetector::FindClosestXFoodIdx(core::Cell &cell, core::FoodStorage &foods) {
//  if (foods.empty()) return {};
//  return BinarySearchClosestFoodX(
//      foods,
//      0,
//      foods.size() - 1,
//      cell.GetPosition().x,
//      std::numeric_limits<float>::max(),
//      0
//  );
//}

void CollisionResolver::ResolveCollisions(core::Cell &cell_1, core::Cell &cell_2) {
  auto radius_sum = cell_1.GetSize() + cell_2.GetSize();
  auto diff_vector = cell_1.GetPosition() - cell_2.GetPosition();
  auto distance_to_move =
      radius_sum - diff_vector.Magnitude() + CollisionResolver::k_collision_distance_padding_;
  auto direction = diff_vector;
  direction.Normalize();

  cell_1.GetPosition() += direction * (distance_to_move * -1.0f * (cell_1.GetSize() / radius_sum));
  cell_2.GetPosition() += direction * (distance_to_move * (cell_2.GetSize() / radius_sum));
}
}
