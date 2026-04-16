#include "collectible_game_object.h"

namespace game {

	// Sets the value granted on pickup based on type
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, CollectibleType type, bool hasGravity, glm::vec2 scale)
		: GameObject(position, geom, shader, texture, scale), type_(type), value_(0), hasGravity_(hasGravity) {
		switch (type) {
		case CollectibleType::Shell:  value_ = 10; break;
		case CollectibleType::Health: value_ = 1;  break;
		case CollectibleType::Ammo:   value_ = 3;  break;
		case CollectibleType::Melee:  value_ = 10; break;
		}
	}

	// Only apply physics (gravity, floor collision) if the item was dropped by an enemy
	void CollectibleGameObject::Update(double delta_time) {
		if (hasGravity_)
			GameObject::Update(delta_time);
	}

}