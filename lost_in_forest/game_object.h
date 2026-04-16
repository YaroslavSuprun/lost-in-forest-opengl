#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

#include "shader.h"
#include "geometry.h"
#include "timer.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {
    public:
        // Constructor
        GameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale = glm::vec2(1, 1), float hDamping = 0.0005, float gravity = 8);

        // Update the GameObject's state. Can be overriden in children
        virtual void Update(double delta_time);

        // Renders the GameObject. Can also be overriden in children
        virtual void Render(glm::mat4 view_matrix, double current_time, glm::mat4 parent_matrix = glm::mat4(1.0f), glm::mat4 offset_matrix = glm::mat4(1.0f));

        // Getters
        //
        // Get position in OpenGL coordinates
        inline glm::vec3 GetPosition(void) const { return position_; }
        // Get scale (size) of the object
        inline glm::vec2 GetScale(void) const { return scale_; }
        // Get current orientation
        inline float GetRotation(void) const { return angle_; }

        // Get bearing direction (vector pointing to the direction in which
        // the game object is facing)
        glm::vec3 GetBearing(void) const;

        // Get vector pointing to the right side of the game object
        glm::vec3 GetRight(void) const;

        // Setters
        inline void SetPosition(const glm::vec3& position) { position_ = position; }
        inline void SetScale(glm::vec2& scale) { scale_ = scale; }
        inline void SetScale(float scale) { scale_ = glm::vec2(scale); }
        void SetRotation(float angle);
        void SetTexture(GLuint& texture) { texture_ = texture; }

        // Movement helpers
        void AddVelocity(const glm::vec3& dv) { velocity_ += dv; }
        void SetVelocity(const glm::vec3& dv) { velocity_ = dv; }
        glm::vec3 GetVelocity() const { return velocity_; }
        void ResetVelocity() { velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); }
        void UpdateVelocity(double delta_time);
        void Rotate(float delta_angle);

        void SetGravity(float gr) { gravity_ = gr; }
        void SetHDamping(float hd) { hDamping_ = hd; }

        void SetDirection(float dr) { direction_ = dr; }
        float GetDirection() const { return direction_; }

        bool OnGround() { return onGround_; }
        void MarkInAir() { onGround_ = false; }
        void MarkOnGround() { onGround_ = true; }

        // Deferred removal - marked objects are cleaned up at end of frame
        bool IsMarkedForRemoval() { return toRemove_; }
        void MarkForRemoval() { toRemove_ = true; }

        // Background tiling controls
        void SetTilingFactor(const glm::vec2& factor) { tiling_factor_ = factor; }
        void SetUVOffset(const glm::vec2& offset) { uv_offset_ = offset; }

        // Transformation matrices for hierarchical rendering
        glm::mat4 GetScalingMatrix() const { return scaling_matrix_; }
        glm::mat4 GetRoationMatrix() const { return rotation_matrix_; }
        glm::mat4 GetTranslationMatrix() const { return translation_matrix_; }
        glm::mat4 GetTransformationMatrix() const { return transformation_matrix_; }


    protected:
        // Transform
        glm::vec3 position_;
        glm::vec2 scale_;
        float angle_;

        bool toRemove_;

        // Physics
        glm::vec3 velocity_;
        float hDamping_;    // Horizontal velocity damping factor (applied per frame via pow)
        float gravity_;     // Downward acceleration when airborne
        bool onGround_;     // Whether the object is currently on a floor surface
        float direction_;   // Facing direction: 1 = right, -1 = left

        // UV tiling for repeating textures (floors, backgrounds)
        glm::vec2 tiling_factor_;
        glm::vec2 uv_offset_;

        // Geometry
        Geometry* geometry_;

        // Shader
        Shader* shader_;

        // Object's texture reference
        GLuint texture_;

        // Cached transformation matrices, rebuilt each frame in Render
        glm::mat4 scaling_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 translation_matrix_;
        glm::mat4 transformation_matrix_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_