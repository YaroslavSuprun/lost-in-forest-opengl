#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"

namespace game {

    // Particle effect used for explosions (red-orange burst) and dirt (brown puffs)
    class ParticleSystem : public GameObject {
    public:
        ParticleSystem(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        void Update(double delta_time) override;
        void Render(glm::mat4 view_matrix, double current_time, glm::mat4 parent_matrix = glm::mat4(1.0f), glm::mat4 offset_matrix = glm::mat4(1.0f)) override;

        void StartLifetime(double duration) { lifetime_.Start(duration); }
        bool IsFinished() const { return lifetime_.Finished(); }
        void SetBirthTime(double t) { birth_time_ = t; }

    private:
        double birth_time_;  // Time of creation, passed to shader for animation
        Timer lifetime_;     // Auto-removes the effect after duration expires

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_