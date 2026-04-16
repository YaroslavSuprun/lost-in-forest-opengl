#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "floor_game_object.h"

namespace game {

    // The player character - a cat in a box, cursed and lost in a forest
    class PlayerGameObject : public GameObject {
        int health_;
        int ammo_;
        int score_;
        bool wasOnGround_;
        FloorGameObject* currentFloor_;  // Floor the player is currently standing on, used for enemy aggro checks
        Timer meleeBoostTimer_;          // Tracks remaining duration of melee range power-up
        bool invincible_;                // Brief invincibility after taking damage
        Timer invincibilityTimer_;
        bool gunUnlocked_;               // Set to true after defeating the first boss
        bool shell1Owned_;               // Sacred shell from boss 2
        bool shell2Owned_;               // Sacred shell from boss 3

    public:
        PlayerGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale = glm::vec2(1, 1));
        void Update(double delta_time) override;

        // Health
        void SetHealth(int health) { health_ = health; }
        int GetHealth() const { return health_; }
        void GetHit(int damage);
        void Heal(int h) { health_ += h; }
        bool IsAlive() { return health_ > 0; }

        // Ammo
        int GetAmmo() const { return ammo_; }
        void AddAmmo(int a) { ammo_ += a; }
        void Shoot() { ammo_--; }

        // Score
        int GetScore() const { return score_; }
        void AddScore(int s) { score_ += s; }

        // Ground tracking
        bool WasOnGround() const { return wasOnGround_; }
        void SetWasOnGround(bool a) { wasOnGround_ = a; }
        FloorGameObject* GetCurrentFloor() { return currentFloor_; }
        void SetCurrentFloor(FloorGameObject* f) { currentFloor_ = f; }

        // Power-ups and progression flags
        Timer& GetMeleeBoostTimer() { return meleeBoostTimer_; }
        bool GetGunUnlocked() const { return gunUnlocked_; }
        bool GetShell1Owned() const { return shell1Owned_; }
        bool GetShell2Owned() const { return shell2Owned_; }
        void SetGunUnlocked() { gunUnlocked_ = true; }
        void SetShell1Owned() { shell1Owned_ = true; }
        void SetShell2Owned() { shell2Owned_ = true; }

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_