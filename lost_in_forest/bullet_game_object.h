#ifndef BULLET_GAME_OBJECT_H_
#define BULLET_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Projectile fired by the player or turret enemies
	class BulletGameObject : public GameObject {
		float lifespanSec_;        // How long the bullet lives before auto-removal
		Timer lifeTimer_;          // Tracks remaining lifespan
		glm::vec3 prevPos_;        // Position from last frame, used for ray-based collision
		bool isPlayerBullet_;      // Distinguishes player bullets from enemy bullets for collision

	public:
		BulletGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, bool isPlayerBullet, glm::vec2 scale = glm::vec2(1, 1), float angle = 0);
		void Update(double delta_time) override;

		glm::vec3 GetPrevPos() { return prevPos_; }
		bool IsPlayerBullet() const { return isPlayerBullet_; }
	};

}

#endif