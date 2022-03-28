#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"

#include <vector>
#include <random>

namespace cells_evo::logic {
    Logic::Logic(core::World &world) : world_(world) {
        RebuildCellsFoodCache();
    }

    void Logic::WorldTick() {
        ProcessEvents();
        MoveCells();
        CheckCellsEnergy();
        GenerateFood();
        DivideCells();
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
            RebuildCellsFoodCache();
        }
    }

    void Logic::CheckCellsEnergy() {
        std::vector<unsigned int> cells_to_kill{};
        for (auto &[_, cell]: world_.cells_) {
            if (!cell.HasEnergy()) cells_to_kill.push_back(cell.id_);
        }
        for (auto cell_id: cells_to_kill) {
            KillCell(cell_id);
        }
    }

    void Logic::KillCell(unsigned int cell_id) {
        cell_food_cache_.erase(cell_id);
        world_.cells_.erase(cell_id);
    }

    void Logic::DivideCells() {

    }

    void Logic::RebuildCellsFoodCache() {
        cell_food_cache_.clear();
        for (auto&[_, cell]: world_.cells_) {
            FindClosestFood(cell);
        }
    }

    void Logic::ProcessEvents() {
        for (auto&[_, cell]: world_.cells_) {
            // todo what if two cells eat simultaneously?
            ProcessEatFood(cell);
        }
    }

    void Logic::ProcessEatFood(core::Cell &cell) {
        auto food = FindClosestFood(cell);
        if (food && CellGotFood(cell, food.value())) {
            cell.AddEnergy(core::Food::k_default_energy_value_);
            RemoveFoodFromCache(food.value());
            world_.food_.erase(food.value().id_);
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

    void Move(core::Cell &cell, core::Vector2<float> &direction, float speed) {
        auto position = cell.GetPosition();
        // todo implement vector addition and multiplication
        auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
        cell.SetPosition(new_pos);
        cell.ConsumeEnergy();
    }

    void Logic::MoveCells() {
        for (auto&[_, cell]: world_.cells_) {
            auto closest_food = FindClosestFood(cell);
            if (!closest_food) return;
            auto target_position = closest_food.value().GetPosition();
            auto direction = core::GetDirectionVector(cell.GetPosition(), target_position);
            Move(cell, direction, cell.speed_);
        }
    }

    // todo I want it to return a reference
    std::optional<core::Food> Logic::FindClosestFood(core::Cell &cell) {
        if (world_.food_.empty())
            return {};
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
