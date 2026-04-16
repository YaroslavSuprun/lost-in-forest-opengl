#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.h"


namespace game {

ParticleSystem::ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture){
    birth_time_ = -1.0;
}


void ParticleSystem::Update(double delta_time) {

    // Call the parent's update method to move the object in standard way, if desired
    GameObject::Update(delta_time);
}


void ParticleSystem::Render(glm::mat4 view_matrix, double current_time, glm::mat4 parent_matrix, glm::mat4 offset_matrix){

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = parent_matrix * offset_matrix * translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set the time in the shader
    shader_->SetUniform1f("time", current_time);

    if (birth_time_ >= 0.0) {
        shader_->SetUniform1f("birth_time", (float)birth_time_);
    }

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the particle texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
