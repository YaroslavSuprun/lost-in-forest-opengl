#ifndef PATROLLER_ENEMY_H_
#define PATROLLER_ENEMY_H_

#include "enemy_game_object.h"

namespace game {

	// Enemy that walks back and forth within its parent floor bounds
	class PatrollerEnemy : public EnemyGameObject {
	public:
		PatrollerEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, FloorGameObject* parentFloor, bool isBoss = false, glm::vec2 scale = glm::vec2(1.5f, 1.5f));
		void Update(double delta_time) override;
	};

}

#endif