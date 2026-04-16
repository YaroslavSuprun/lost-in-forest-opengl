#include "enemy_game_object.h"

namespace game {

	// Initializes enemy with HP, patrol bounds, and optional boss scaling
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss, glm::vec2 scale)
		: GameObject(position, geom, shader, texture, scale), parentFloor_(parentFloor), isBoss_(isBoss), state_(EnemyState::PATROL), health_(3), margin_(1.5) {
		// Boss enemies get triple HP and double visual size
		if (isBoss_) {
			health_ *= 3;
			SetScale(GetScale() * 2.0f);
		}
		// Calculate patrol boundaries from parent floor dimensions
		leftBound = parentFloor_->GetPosition().x - parentFloor_->GetScale().x * 0.5f + margin_;
		rightBound = parentFloor_->GetPosition().x + parentFloor_->GetScale().x * 0.5f - margin_;
	}

	// Base update applies gravity and damping from GameObject
	void EnemyGameObject::Update(double delta_time) {
		GameObject::Update(delta_time);
	}

}