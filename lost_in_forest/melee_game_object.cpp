#include "melee_game_object.h"

namespace game {

	// Spawns at the player's position, offset in the direction they're facing
	MeleeGameObject::MeleeGameObject(Geometry* geom, Shader* shader, GLuint texture, GameObject* parent, float offset, glm::vec2 scale)
		: GameObject(parent->GetPosition(), geom, shader, texture, scale), parent_(parent), offset_(offset), lifespanSec_(0.2) {
		this->SetDirection(parent->GetDirection());
		lifeTimer_.Start(lifespanSec_);
		SetOffsetPosition();
	}

	void MeleeGameObject::Update(double delta_time) {
		SetOffsetPosition();  // Follow the player each frame
		if (lifeTimer_.Finished())
			this->MarkForRemoval();
	}

	// Position the slash hitbox in front of the player based on facing direction
	void MeleeGameObject::SetOffsetPosition() {
		glm::vec3 parent_pos = parent_->GetPosition();
		if (direction_ == 1) {
			this->SetPosition(glm::vec3(parent_pos.x + offset_, parent_pos.y, 0.0));
		}
		else {
			this->SetPosition(glm::vec3(parent_pos.x - offset_, parent_pos.y, 0.0));
		}
	}

}