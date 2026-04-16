#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Types of collectible items in the game
	enum class CollectibleType {
		Shell,   // Adds to score
		Health,  // Restores player HP
		Ammo,    // Replenishes ranged ammo
		Melee    // Temporary melee range boost power-up
	};

	// Pickups that spawn in the world or drop from enemies on kill
	class CollectibleGameObject : public GameObject {
		CollectibleType type_;
		int value_;        // Amount granted on pickup (score, HP, or ammo)
		bool hasGravity_;  // True for enemy drops (fall to ground), false for pre-placed items

	public:
		CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, CollectibleType type, bool hasGravity, glm::vec2 scale = glm::vec2(1, 1));
		void Update(double delta_time) override;

		CollectibleType GetType() const { return type_; }
		int GetValue() const { return value_; }
	};

}

#endif