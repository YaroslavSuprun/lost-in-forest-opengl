#include "patroller_enemy.h"

namespace game {

	PatrollerEnemy::PatrollerEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss, glm::vec2 scale)
		: EnemyGameObject(position, geom, shader, texture, parentFloor, isBoss, scale) {
	}

	void PatrollerEnemy::Update(double delta_time) {
		// Reverse direction when reaching patrol boundary
		if (position_.x < leftBound || position_.x > rightBound)
			direction_ *= -1;
		// Constant horizontal movement (negated due to sprite facing direction)
		velocity_.x = -3.5f * direction_;
		EnemyGameObject::Update(delta_time);
	}

}