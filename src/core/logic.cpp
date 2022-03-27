#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"

namespace cells_evo::logic {
    Logic::Logic(core::World &world) : world(world) {
    }

    void Logic::WorldTick() {
        for (auto&[_, cell]: this->world.cells) {
            this->CellAct(cell);
        }
    }

    void Move(core::Entity &entity, core::Vector2<float> &direction, float speed) {
        auto position = entity.GetPosition();
        auto new_pos = core::Position(position.X() + (direction.x * speed), position.Y() + (direction.y * speed));
        entity.SetPosition(new_pos);
    }

    void Logic::CellAct(core::Cell &cell) {
        auto target_position = this->GetClosestFood(cell).GetPosition();
        auto direction = core::GetDirectionVector(cell.GetPosition(), target_position);
        Move(cell, direction, cell.speed);
    }

    core::Food &Logic::GetClosestFood(core::Cell &cell) {
        if (this->world.food.empty())
            // todo return optional
            throw std::runtime_error("No food exists!");
        int closest_idx;
        float min_distance = std::numeric_limits<float>::max();
        for (auto&[idx, food]: this->world.food) {
            // todo seems position makes life harder
            auto dist = (food.GetPosition() - cell.GetPosition()).coordinates.Magnitude();
            if (dist < min_distance) {
                min_distance = dist;
                closest_idx = idx;
            }
        }
        return this->world.food.find(closest_idx)->second;
    }
}
