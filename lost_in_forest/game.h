#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "game_object.h"
#include "player_game_object.h"
#include "enemy_game_object.h"
#include "patroller_enemy.h"
#include "turret_enemy.h"
#include "dasher_enemy.h"
#include "floor_game_object.h"
#include "collectible_game_object.h"
#include "bullet_game_object.h"
#include "melee_game_object.h"
#include "particle_system.h"
#include "particles.h"


namespace game {

    // Result of a circle-AABB collision test
    struct CollisionResult {
        bool collided = false;
        bool onGround = false;
    };

    // Main game class - manages the game loop, world setup, input, rendering, and all game logic
    class Game {
        // Texture indices matching the order textures are loaded in SetupGameWorld
        enum TextureIndex {
            tex_tileable_sky = 0,
            tex_catInBox,
            tex_hand,
            tex_fish1,
            tex_squid,
            tex_clownfish,
            tex_shell,
            tex_shell1,
            tex_shell2,
            tex_crab,
            tex_jellyfish,
            tex_shrimp,
            tex_blob,
            tex_flower,
            tex_tileable_grass,
            tex_orb,
            tex_slash,
            tex_bg_far,
            tex_bg_mid,
            tex_bg_front,

            tex_numbers,
            tex_text_intro,
            tex_noShellsText,
            tex_gunUnlockText,
            tex_winText,
            tex_passageThoughtText
        };

    public:
        Game(void);
        ~Game();

        void Init(void);
        void SetupGameWorld(void);
        void DestroyGameWorld(void);
        void MainLoop(void);

    private:
        bool debug_mode_;

        // --- Window and rendering ---
        GLFWwindow* window_;
        Geometry* sprite_;
        Shader sprite_shader_;
        GLuint* tex_;

        // --- Particle systems ---
        Shader explosion_shader_;
        Particles* explosion_geom_;
        std::vector<ParticleSystem*> explosions_;
        Shader dirt_shader_;
        Particles* dirt_geom_;
        std::vector<ParticleSystem*> dirt_;
        Timer dirtCdTimer_;

        // --- Core game objects ---
        PlayerGameObject* player;
        GameObject* player_hand;   // Orbits player using hierarchical transform, aims at mouse
        GameObject* background_far;
        GameObject* background_mid;
        GameObject* background_front;

        // --- HUD icons ---
        GameObject* health_icon;
        GameObject* ammo_icon;
        GameObject* score_icon;
        GameObject* shell1_icon;
        GameObject* shell2_icon;

        // --- Game object collections ---
        std::vector<EnemyGameObject*> enemy_game_objects_;
        std::vector<FloorGameObject*> floor_game_objects_;
        std::vector<CollectibleGameObject*> collectible_game_objects_;
        std::vector<BulletGameObject*> bullet_game_objects_;
        std::vector<MeleeGameObject*> melee_game_objects_;

        // --- Bosses (named pointers for tracking kill flags) ---
        PatrollerEnemy* bossPatroller;   // Drops gun unlock on death
        TurretEnemy* bossTurret;         // Drops sacred shell 1 on death
        DasherEnemy* bossDasher;         // Drops sacred shell 2 on death

        // --- NPCs and dialogue ---
        GameObject* friendlyFish;
        GameObject* introText;
        GameObject* gatekeeper;          // Blocks exit until both shells collected
        GameObject* midGatekeeper;       // Blocks passage until player has visited gatekeeper
        bool passage;                    // Set true after player first approaches gatekeeper

        // --- Popup text sprites ---
        GameObject* gunUnlockText;
        Timer gunUnlockTextTimer;
        GameObject* noShellsText;
        GameObject* winText;
        Timer winTimer;
        Timer passageThoughtTimer;
        GameObject* passageThoughtText;

        // --- Game state ---
        double current_time_;
        bool gameOver_;

        // --- Camera ---
        float camera_zoom_;
        glm::vec2 camera_offset_;

        glm::vec3 player_start_pos_;

        // --- Player attack cooldowns ---
        float shotCdSec_;
        Timer shotCdTimer_;
        float meleeCdSec_;
        Timer meleeCdTimer_;
        float aimAngle_;

        // --- Darkening / nightfall mechanic ---
        bool darkening_;
        float currentDarkness_;
        float timeLimit_;
        float darknessStart_;   // Time when screen starts darkening
        float darknessEnd_;     // Time when darkness reaches maximum
        Timer gameOverTimer_;

        // --- Core game loop methods ---
        void HandleControls(double delta_time);
        void Update(double delta_time);
        void Render(void);

        bool GetMousePosition(glm::vec2& mouse);
        static void ResizeCallback(GLFWwindow* window, int width, int height);

        // --- Texture loading ---
        void SetTexture(GLuint w, const char* fname);
        void LoadTextures(std::vector<std::string>& textures);

        // --- Update helpers ---
        void HandleEnemyUpdates(double delta_time);
        void HandleCollisions();
        void HandleBackground();

        // --- Collision detection ---
        // Ray-circle: tests if bullet path intersects an enemy's hit radius
        bool RayToCircleCollision(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& C, float radius);
        // Circle-circle: overlap test for body collisions and pickups
        bool CircleCollision(GameObject*, float r1, GameObject*, float r2);
        // Circle-AABB: player/enemy vs floor, with collision resolution and ground detection
        CollisionResult CircleAABBCollision(GameObject* circle, FloorGameObject* floor, float radius);

        // --- Utility ---
        void DebugPrintPlayer(bool pos, bool vel);
        void ResetPlayerPositionOnFall();
        void SpawnExplosion(glm::vec3 position);
        void SpawnDirt(glm::vec3 position);
        void PopulateFloor(FloorGameObject* floor, int patrollers, int turrets, int dashers, int shells);
        void Game::RenderNumber(int value, glm::vec3 position, float digitSize, glm::mat4 view_matrix);
        void RenderHUD(glm::mat4 view_matrix, double current_time);

        // --- Template helpers for managing object vectors ---
        template <typename T>
        void FreeVector(std::vector<T*>& vec) {
            for (int i = 0; i < vec.size(); ++i)
                delete vec[i];
        }

        // Removes and deletes objects flagged for removal (iterated back-to-front for safe erasure)
        template <typename T>
        void RemoveMarkedFromVector(std::vector<T*>& vec) {
            for (int i = vec.size() - 1; i >= 0; --i)
                if (vec[i]->IsMarkedForRemoval()) {
                    delete vec[i];
                    vec.erase(vec.begin() + i);
                }
        }

        template <typename T>
        void UpdateVector(std::vector<T*> vec, double delta_time) {
            for (int i = 0; i < vec.size(); ++i)
                vec[i]->Update(delta_time);
        }

        template <typename T>
        void RenderVector(std::vector<T*>& vec, glm::mat4 view_matrix, double current_time) {
            for (int i = 0; i < vec.size(); i++)
                vec[i]->Render(view_matrix, current_time);
        }

    }; // class Game

} // namespace game

#endif // GAME_H_