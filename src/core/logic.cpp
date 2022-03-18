#include "CellsEvo/core/logic.h"
#include "CellsEvo/core/geometry.h"

namespace cells_evo::logic {
    Logic::Logic(core::World &world) : world(world) {
    }

    void Logic::WorldTick() {
        for (auto [_, cell]: this->world.cells) {
            this->CellAct(cell);
        }
    }

    // todo take account of time
    void Move(core::Entity& entity, core::Vector2<float> direction, float speed) {
        auto position = entity.GetPosition();
        auto new_pos = core::Position(position.X() + (direction.x * speed), position.X() + (direction.x * speed));
        entity.SetPosition(new_pos);
    }

    void Logic::CellAct(core::Cell &cell) {
        auto target_position = this->GetClosestFood().GetPosition();
        auto direction = core::GetDirectionVector(cell.GetPosition(), target_position);
        Move(cell, direction, cell.speed);
    }

    core::Food& Logic::GetClosestFood() {
        auto res = this->world.food.find(0);
        if (res != this->world.food.end()) {
            return res->second;
        }
        // todo
        throw "No food exists!";
    }
}
