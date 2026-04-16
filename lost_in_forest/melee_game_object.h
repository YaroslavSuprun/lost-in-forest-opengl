#ifndef MELEE_GAME_OBJECT_H_
#define MELEE_GAME_OBJECT_H_

#include "game_object.h"
#include "enemy_game_object.h"
#include <set>

namespace game {

	// Slash attack that follows the player, damages enemies on contact
	class MeleeGameObject : public GameObject {
		float lifespanSec_;
		Timer lifeTimer_;
		GameObject* parent_;                     // Player this slash is attached to
		float offset_;                           // Distance from player center
		std::set<EnemyGameObject*> hit_enemies_; // Tracks which enemies were already hit to prevent double damage

	public:
		MeleeGameObject(Geometry* geom, Shader* shader, GLuint texture, GameObject* parent, float offset, glm::vec2 scale = glm::vec2(1, 1));
		void Update(double delta_time) override;
		void SetOffsetPosition();

		bool HasHit(EnemyGameObject* e) { return hit_enemies_.count(e) > 0; }
		void RegisterHit(EnemyGameObject* e) { hit_enemies_.insert(e); }
	};

}

#endif