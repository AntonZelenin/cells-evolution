#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"

#include <vector>
#include <random>

namespace cells_evo::logic {
    Logic::Logic(core::World &world) : world_(world) {
    }

    void Logic::WorldTick() {
        BuildCellsFoodCache();

        ProcessEvents();
        MoveCells();
        GenerateFood();
        DivideCells();

        ClearCache();
    }

    void Logic::GenerateFood() {
        double secs = 2;
        // todo this is not good
        int frames = static_cast<int>(secs / 0.016666);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0, frames);
        // generate food only once in secs on average
        if (distribution(generator) == 1) {
            world_.GenerateFood(1);
        }
    }

    void Logic::DivideCells() {

    }

    void Logic::BuildCellsFoodCache() {
        for (auto&[_, cell]: world_.cells_) {
            FindClosestFood(cell);
        }
    }

    void Logic::ClearCache() {
        cell_food_cache_.clear();
    }

    void Logic::ProcessEvents() {
        for (auto&[_, cell]: world_.cells_) {
            // todo what if two cells eat simultaneously?
            ProcessEatFood(cell);
        }
    }

    void Logic::ProcessEatFood(core::Cell &cell) {
        auto food = FindClosestFood(cell);
        if (CellGotFood(cell, food)) {
            RemoveFoodFromCache(food);
            world_.food_.erase(food.id_);
        }
    }

    void Logic::RemoveFoodFromCache(const core::Food &food) {
        std::vector<unsigned int> cells_to_remove{};
        for (auto[cell_id, food_id]: cell_food_cache_) {
            if (food.id_ == food_id) cells_to_remove.push_back(cell_id);
        }
        for (auto cell_id: cells_to_remove) {
            cell_food_cache_.erase(cell_id);
        }
    }

    bool Logic::CellGotFood(core::Cell &cell, core::Food &food) {
        return (cell.GetPosition() - food.GetPosition()).Magnitude() < cell.size_;
    }

    void Logic::MoveCells() {
        for (auto&[_, cell]: world_.cells_) {
            CellAct(cell);
        }
    }

    void Move(core::Entity &entity, core::Vector2<float> &direction, float speed) {
        auto position = entity.GetPosition();
        auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
        entity.SetPosition(new_pos);
    }

    void Logic::CellAct(core::Cell &cell) {
        auto target_position = FindClosestFood(cell).GetPosition();
        auto direction = core::GetDirectionVector(cell.GetPosition(), target_position);
        Move(cell, direction, cell.speed_);
    }

    core::Food &Logic::FindClosestFood(core::Cell &cell) {
        if (world_.food_.empty())
            // todo return optional
            throw std::runtime_error("No food exists!");
        if (cell_food_cache_.find(cell.id_) != cell_food_cache_.end())
            return world_.food_.find(cell_food_cache_.find(cell.id_)->second)->second;
        unsigned int closest_food_idx;
        float min_distance = std::numeric_limits<float>::max();
        for (auto&[idx, food]: world_.food_) {
            // todo seems position makes life harder
            auto dist = (food.GetPosition() - cell.GetPosition()).Magnitude();
            if (dist < min_distance) {
                min_distance = dist;
                closest_food_idx = idx;
            }
        }

        cell_food_cache_.insert({cell.id_, closest_food_idx});
        return world_.food_.find(closest_food_idx)->second;
    }
}
