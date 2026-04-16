#include "turret_enemy.h"

namespace game {

	TurretEnemy::TurretEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss, glm::vec2 scale)
		: EnemyGameObject(position, geom, shader, texture, parentFloor, isBoss, scale), cdTimeSec_(1.5f), shotDone_(false) {
	}

	void TurretEnemy::Update(double delta_time) {
		switch (state_) {
		case game::AGGRO:
			// After Game creates a bullet, restart the shot cooldown
			if (shotDone_) {
				cdTimer_.Start(cdTimeSec_);
				shotDone_ = false;
			}
			break;
		default:
			// Turret is stationary, does nothing when not aggro'd
			break;
		}
	}

}