#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>
#include "game_object.h"
#include "floor_game_object.h"

namespace game {

	// Possible AI states for enemies
	enum EnemyState { PATROL, AGGRO };

	// Base class for all enemy types (patroller, turret, dasher)
	// Handles HP, boss scaling, patrol bounds, and parent floor reference
	class EnemyGameObject : public GameObject {
		bool isBoss_;      // If true, enemy has 3x HP and 1.5x scale
		float margin_;     // Inset from floor edges for patrol bounds

	protected:
		EnemyState state_;
		int health_;
		FloorGameObject* parentFloor_;  // Floor this enemy belongs to, used for patrol bounds and aggro checks
		float leftBound;                // Left patrol boundary (derived from parent floor)
		float rightBound;               // Right patrol boundary (derived from parent floor)

	public:
		EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss = false, glm::vec2 scale = glm::vec2(1.5f, 1.5f));
		void Update(double delta_time) override;

		void SetHealth(int health) { health_ = health; }
		int GetHealth() const { return health_; }
		void GetHit(int dmg) { health_ -= dmg; }
		bool IsAlive() { return health_ > 0; }
		FloorGameObject* GetParentFloor() const { return parentFloor_; }
		EnemyState& GetState() { return state_; }
		void SetState(EnemyState s) { state_ = s; }
	};

}

#endif