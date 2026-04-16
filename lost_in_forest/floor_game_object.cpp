#include "floor_game_object.h"

namespace game {

	// Creates a floor platform by tiling the grass texture across the specified dimensions
	FloorGameObject::FloorGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 tileNum, float angle)
		: GameObject(position, geom, shader, texture), tileNum_(tileNum), tileSize_(1.8) {
		SetScale(glm::vec2(tileNum_.x * tileSize_, tileNum_.y * tileSize_));  // Total size = tile count * tile size
		this->SetRotation(angle);
		SetTilingFactor(glm::vec2(tileNum_.x, tileNum_.y));  // Repeat texture across the surface
	}

	// Floors are static, no update needed
	void FloorGameObject::Update(double delta_time) {}

}