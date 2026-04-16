#ifndef FLOOR_GAME_OBJECT_H_
#define FLOOR_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Static platform tile that serves as ground for the player and enemies
	class FloorGameObject : public GameObject {
		float tileSize_;    // Size of each individual tile in world units
		glm::vec2 tileNum_; // Number of tiles horizontally and vertically

	public:
		FloorGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 tileNum, float angle = 0);
		void Update(double delta_time) override;
	};

}

#endif