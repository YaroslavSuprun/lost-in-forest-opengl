#ifndef TURRET_ENEMY_H_
#define TURRET_ENEMY_H_

#include "enemy_game_object.h"

namespace game {

	// Stationary enemy that shoots projectiles at the player when aggro'd
	class TurretEnemy : public EnemyGameObject {
		float cdTimeSec_;   // Seconds between shots
		Timer cdTimer_;     // Cooldown between shots
		bool shotDone_;     // Signals the turret to restart its cooldown

	public:
		TurretEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss = false, glm::vec2 scale = glm::vec2(1.5f, 1.5f));
		void Update(double delta_time) override;

		Timer& GetCdTimer() { return cdTimer_; }
		void ShotDone() { shotDone_ = true; }
	};

}

#endif