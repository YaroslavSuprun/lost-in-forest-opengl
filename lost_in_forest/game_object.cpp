#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "game_object.h"

namespace game {

    GameObject::GameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale, float damping, float gravity) {
        // Initialize all attributes
        position_ = position;
        scale_ = scale;
        angle_ = 0.0;
        velocity_ = glm::vec3(0.0f);
        hDamping_ = damping;
        gravity_ = gravity;
        geometry_ = geom;
        shader_ = shader;
        texture_ = texture;
        toRemove_ = false;
        onGround_ = false;
        tiling_factor_ = glm::vec2(1.0f, 1.0f);
        uv_offset_ = glm::vec2(0);
        direction_ = 1;
    }

    glm::vec3 GameObject::GetBearing(void) const {
        glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
        return dir;
    }

    glm::vec3 GameObject::GetRight(void) const {
        float pi_over_two = glm::pi<float>() / 2.0f;
        glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
        return dir;
    }

    void GameObject::SetRotation(float angle) {
        // Set rotation angle of the game object
        // Make sure angle is in the range [0, 2*pi]
        float two_pi = 2.0f * glm::pi<float>();
        angle = fmod(angle, two_pi);
        if (angle < 0.0) {
            angle += two_pi;
        }
        angle_ = angle;
    }

    void GameObject::Update(double delta_time) {
        UpdateVelocity(delta_time);
    }

    void GameObject::Render(glm::mat4 view_matrix, double current_time, glm::mat4 parent_matrix, glm::mat4 offset_matrix) {
        // Set up the shader
        shader_->Enable();
        // Set up the view matrix
        shader_->SetUniformMat4("view_matrix", view_matrix);
        // Scaling matrix flips horizontally based on direction for sprite facing
        scaling_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x * direction_, scale_.y, 1.0));
        // Setup the rotation matrix for the shader
        rotation_matrix_ = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
        // Set up the translation matrix for the shader
        translation_matrix_ = glm::translate(glm::mat4(1.0f), position_);
        // Full transform: parent and offset matrices enable hierarchical rendering (e.g. hand orbiting player)
        transformation_matrix_ = parent_matrix * offset_matrix * translation_matrix_ * rotation_matrix_ * scaling_matrix_;
        // Set the transformation matrix in the shader
        shader_->SetUniformMat4("transformation_matrix", transformation_matrix_);
        // Set up the geometry
        geometry_->SetGeometry(shader_->GetShaderProgram());
        // Tiling and UV offset for repeating textures (floors, backgrounds)
        shader_->SetUniform2f("tiling_factor", tiling_factor_);
        shader_->SetUniform2f("uv_offset", uv_offset_);
        // Bind the entity's texture
        glBindTexture(GL_TEXTURE_2D, texture_);
        // Draw the entity
        glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
    }

    // Apply horizontal damping, gravity when airborne, and integrate position
    void GameObject::UpdateVelocity(double delta_time) {
        velocity_.x *= pow(hDamping_, (float)delta_time);
        if (!OnGround())
            velocity_.y -= gravity_ * (float)delta_time;
        position_ += velocity_ * (float)delta_time;
    }

    void GameObject::Rotate(float delta_angle) {
        SetRotation(angle_ + delta_angle);
    }

} // namespace game