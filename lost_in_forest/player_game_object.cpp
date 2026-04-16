#include "player_game_object.h"
#include <iostream>

namespace game {

	PlayerGameObject::PlayerGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale)
		: GameObject(position, geom, shader, texture, scale), health_(8), ammo_(12), score_(0), wasOnGround_(false), currentFloor_(nullptr), invincible_(false), gunUnlocked_(false), shell1Owned_(false), shell2Owned_(false) {
		MarkInAir();
	}

	void PlayerGameObject::Update(double delta_time) {
		// End invincibility when timer expires
		if (invincible_ && invincibilityTimer_.Finished()) {
			invincible_ = false;
		}
		// Apply gravity and damping
		GameObject::Update(delta_time);
	}

	// Applies damage with brief invincibility window to prevent instant death from repeated hits
	void PlayerGameObject::GetHit(int damage) {
		if (!invincible_) {
			health_ -= damage;
			invincible_ = true;
			invincibilityTimer_.Start(1.5f);
		}
	}

} // namespace game