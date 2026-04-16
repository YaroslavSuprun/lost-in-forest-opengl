#include "dasher_enemy.h"

namespace game {

	DasherEnemy::DasherEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss, glm::vec2 scale)
		: EnemyGameObject(position, geom, shader, texture, parentFloor, isBoss, scale), cdTimeSec_(1.5f), dashDone_(false) {}

    void DasherEnemy::Update(double delta_time) {
        switch (state_) {
        case game::AGGRO:
            // Signal Game to restart the cooldown after a dash was fired
            if (dashDone_) {
                cdTimer_.Start(cdTimeSec_);
                dashDone_ = false;
            }
            // If stopped or hit platform edge, return to patrol
            if (position_.x < leftBound - 0.1f || position_.x > rightBound + 0.1f || (OnGround() && abs(velocity_.x) < 0.5f)) {
                velocity_.x = 0;
                state_ = PATROL;
            }
            break;
        default:
            // Patrol: walk back and forth within platform bounds
            if (position_.x < leftBound || position_.x > rightBound)
                direction_ *= -1;
            velocity_.x = -3.5f * direction_;
            break;
        }

        // Hard clamp to platform edges to prevent overshooting during a dash
        if (position_.x < leftBound - 0.1f) {
            position_.x = leftBound;
            velocity_.x = 0;
            state_ = PATROL;
            direction_ = -1;
        }
        else if (position_.x > rightBound + 0.1f) {
            position_.x = rightBound;
            velocity_.x = 0;
            state_ = PATROL;
            direction_ = 1;
        }

        // Safety check: zero velocity if still outside bounds
        if (position_.x < leftBound - 0.1f || position_.x > rightBound + 0.1f)
            velocity_.x = 0;

        // Apply gravity when airborne; skip horizontal damping during dash (dashTimer active)
        if (!OnGround() || !dashTimer_.Finished()) {
            velocity_.y -= gravity_ * (float)delta_time;
        } else {
            velocity_.x *= pow(hDamping_, (float)delta_time);
        }

        position_ += velocity_ * (float)delta_time;
    }

}