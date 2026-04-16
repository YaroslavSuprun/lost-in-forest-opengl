#ifndef DASHER_ENEMY_H_
#define DASHER_ENEMY_H_

#include "enemy_game_object.h"

namespace game {

	// Enemy that patrols back and forth, then dashes at the player when aggro'd
	class DasherEnemy : public EnemyGameObject {
		float cdTimeSec_;    // Seconds between dashes
		Timer cdTimer_;      // Cooldown between dash attacks
		bool dashDone_;      // Signals Game to restart the cooldown
		Timer dashTimer_;    // Protects dash momentum from ground damping

	public:
		DasherEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss = false, glm::vec2 scale = glm::vec2(1.5f, 1.5f));
		void Update(double delta_time) override;

		Timer& GetCdTimer() { return cdTimer_; }
		void DashDone() { dashDone_ = true; }
		void StartDashTimer(float t) { dashTimer_.Start(t); }
	};

}

#endif