#include "bullet_game_object.h"

namespace game {

	// Bullet has no gravity or damping, flies in a straight line until it expires or hits something
	BulletGameObject::BulletGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, bool isPlayerBullet, glm::vec2 scale, float angle)
		: GameObject(position, geom, shader, texture, scale), lifespanSec_(1), lifeTimer_(Timer()), prevPos_(glm::vec3(-999, -999, 0.0)), isPlayerBullet_(isPlayerBullet) {
		lifeTimer_.Start(lifespanSec_);
		this->SetRotation(angle);
		this->SetHDamping(0);
		this->SetGravity(0);
	}

	void BulletGameObject::Update(double delta_time) {
		if (lifeTimer_.Finished())
			this->MarkForRemoval();
		prevPos_ = position_;  // Store previous position for ray-based collision detection
		position_ += velocity_ * (float)delta_time;
	}

}