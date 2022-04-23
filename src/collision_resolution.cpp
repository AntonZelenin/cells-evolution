#include "CellsEvo/collision_resolution.h"

namespace cells_evo::collisions {
// todo can it be a single struct?
struct cell_less_by_x {
  inline bool operator()(core::Cell &cell_1, core::Cell &cell_2) {
    return cell_1.GetPosition().x < cell_2.GetPosition().x;
  }
};

struct food_less_by_x {
  inline bool operator()(core::Food &food_1, core::Food &food_2) {
    return food_1.GetPosition().x < food_2.GetPosition().x;
  }
};

static bool CellsCollide(core::Cell &cell_1, core::Cell &cell_2) {
  return (cell_1.GetPosition() - cell_2.GetPosition()).MagnitudeSquared()
      < pow((cell_1.GetSize() + cell_2.GetSize()), 2);
}

static bool CellFoodCollide(core::Cell &cell, core::Food &food) {
  return (cell.GetPosition() - food.GetPosition()).MagnitudeSquared()
      < pow((cell.GetSize() + food.GetSize()), 2);
}

IdXPairs CollisionDetector::DetectCellCollisions(core::CellStorage &cells) {
  if (cells.size() < 2)
    return {};

  // todo where is the best place to sort?
  std::sort(cells.begin(), cells.end(), cell_less_by_x());

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
  if (cells.empty() || foods.empty())
    return {};

  // todo lets collision detection will ignore deleted food and cells as a fast solution
  // todo where is the best place to sort?
  std::sort(cells.begin(), cells.end(), cell_less_by_x());
  std::sort(foods.begin(), foods.end(), food_less_by_x());

  FoodCellCollisions colliding;
  for (uint cell_idx = 0; auto &cell : cells) {
    // todo try to improve this check
    if (cell.IsDeleted() || cell.IsDead()) continue;
    // todo was overflow here
    auto closest_x_food_idx_left = CollisionDetector::FindClosestXFoodIdx(cell, foods),
        closest_x_food_idx_right = closest_x_food_idx_left;

    // todo break if distance more then cell.size + food.size
    while (closest_x_food_idx_left >= 0) {
      auto &food = foods.at(closest_x_food_idx_left);
      if (!food.IsDeleted()) {
        // todo early return
        //if (food.GetPosition().x)
        if (CellFoodCollide(cell, food)) {
          colliding.push_back(FoodCellCollision{closest_x_food_idx_left, cell_idx});
        }
      }
      if (closest_x_food_idx_left == 0) break;
      --closest_x_food_idx_left;
    }

    while (++closest_x_food_idx_right < foods.size()) {
      if (foods.at(closest_x_food_idx_right).IsDeleted()) {
        // todo early return
        //if (food.GetPosition().x)
        if (CellFoodCollide(cell, foods.at(closest_x_food_idx_right))) {
          colliding.push_back(FoodCellCollision{closest_x_food_idx_right, cell_idx});
        }
      }
    }
    ++cell_idx;
  }

  return colliding;
}

// todo duplicate
uint BinarySearchClosestFoodX(
    core::FoodStorage &foods,
    int l,
    int r,
    float needle,
    float min_distance,
    uint closest_idx
) {
  if (r >= l) {
    int mid = l + (r - l) / 2;

//    if (foods.at(mid).IsDeleted()) {
      // todo what to do?
//    }

    auto curr_x = foods.at(mid).GetPosition().x;
    auto dist = abs(needle - curr_x);
    if (curr_x == needle)
      return mid;
    if (dist < min_distance) {
      min_distance = dist;
      closest_idx = mid;
    }

    if (curr_x > needle)
      return BinarySearchClosestFoodX(foods, l, mid - 1, needle, min_distance, closest_idx);

    return BinarySearchClosestFoodX(foods, mid + 1, r, needle, min_distance, closest_idx);
  }

  return closest_idx;
}

//uint BinarySearchClosestCellX(
//    core::CellStorage &cells,
//    int l,
//    int r,
//    float needle,
//    float min_distance,
//    uint closest_idx
//) {
//  if (r >= l) {
//    int mid = l + (r - l) / 2;
//
//    auto curr_x = cells.at(mid).GetPosition().x;
//    auto dist = abs(needle - curr_x);
//    if (dist < min_distance) {
//      min_distance = dist;
//      closest_idx = mid;
//    }
//    if (curr_x == needle)
//      return mid;
//
//    if (curr_x > needle)
//      return BinarySearchClosestCellX(cells, l, mid - 1, needle, min_distance, closest_idx);
//
//    return BinarySearchClosestCellX(cells, mid + 1, r, needle, min_distance, closest_idx);
//  }
//
//  return closest_idx;
//}

// todo move
uint CollisionDetector::FindClosestXFoodIdx(core::Cell &cell, core::FoodStorage &foods) {
  // todo foods must be not empty
  return BinarySearchClosestFoodX(
      foods,
      0,
      foods.size() - 1,
      cell.GetPosition().x,
      std::numeric_limits<float>::max(),
      0
  );
}

//// todo only prey cell idx here
//uint CollisionDetector::FindClosestXCellIdx(core::Cell &cell, core::CellStorage &cells) {
//  // todo cells must be not empty
//  return BinarySearchClosestCellX(
//      cells,
//      0,
//      cells.size() - 1,
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
