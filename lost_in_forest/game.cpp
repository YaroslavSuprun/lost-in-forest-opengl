#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <cstdlib>

#include <path_config.h>

#include "sprite.h"
#include "game.h"

namespace game {

    // Window and viewport configuration
    const char* window_title_g = "Lost in Forest";
    const unsigned int window_width_g = 800;
    const unsigned int window_height_g = 600;
    const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

    const std::string resources_directory_g = RESOURCES_DIRECTORY;


    void Game::SetupGameWorld(void)
    {
        // Load all textures in the same order as the TextureIndex enum in game.h
        std::vector<std::string> textures;
        textures.push_back("/textures/tileable_sky.jpg");
        textures.push_back("/textures/catInBox.png");
        textures.push_back("/textures/hand.png");
        textures.push_back("/textures/fish1.png");
        textures.push_back("/textures/squid.png");
        textures.push_back("/textures/clownfish.png");
        textures.push_back("/textures/shell.png");
        textures.push_back("/textures/shell1.png");
        textures.push_back("/textures/shell2.png");
        textures.push_back("/textures/crab.png");
        textures.push_back("/textures/jellyfish.png");
        textures.push_back("/textures/shrimp.png");
        textures.push_back("/textures/blob.png");
        textures.push_back("/textures/flower.png");
        textures.push_back("/textures/tileable_grass_1.png");
        textures.push_back("/textures/orb.png");
        textures.push_back("/textures/slash.png");
        textures.push_back("/textures/background_layer_1.png");
        textures.push_back("/textures/background_layer_2.png");
        textures.push_back("/textures/background_layer_3.png");
        textures.push_back("/textures/numbers.png");
        textures.push_back("/textures/text_intro.png");
        textures.push_back("/textures/noShellsText.png");
        textures.push_back("/textures/gunUnlockText.png");
        textures.push_back("/textures/winText.png");
        textures.push_back("/textures/passageThoughtText.png");
        LoadTextures(textures);

        // Create the player and hand (hand uses hierarchical transform to orbit player)
        player_start_pos_ = glm::vec3(-8.5f, -2.5f, 0.0f);
        player = new PlayerGameObject(player_start_pos_, sprite_, &sprite_shader_, tex_[tex_catInBox], glm::vec2(1.8, 1.8));
        player_hand = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_hand], glm::vec2(0.3f));

        // HUD icons - positioned each frame in RenderHUD
        health_icon = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_clownfish], glm::vec2(1));
        ammo_icon = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_squid], glm::vec2(1));
        score_icon = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_shell], glm::vec2(1));
        shell1_icon = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_shell1], glm::vec2(2));
        shell2_icon = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_shell2], glm::vec2(2));


        // ============ Level layout ============
        // Bottom row: left-to-right progression toward exit

        // Floor 0 - starting platform (player spawns here, friendly NPC fish nearby)
        FloorGameObject* f0 = new FloorGameObject(glm::vec3(-6.0f, -4.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(8, 1));
        floor_game_objects_.push_back(f0);

        // Floor 1 - first encounter with patroller enemies
        FloorGameObject* f1 = new FloorGameObject(glm::vec3(18.0f, -2.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(8, 1));
        floor_game_objects_.push_back(f1);
        PopulateFloor(f1, 2, 0, 0, 3);

        // Floor 1.5 - small heal station before boss 1
        FloorGameObject* f1_2 = new FloorGameObject(glm::vec3(36.0f, -1.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f1_2);
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(36.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(37.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(35.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));

        // Floor 2 - boss 1 (patroller crab), killing it unlocks the ranged weapon
        FloorGameObject* f2 = new FloorGameObject(glm::vec3(60.0f, -4.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(14, 1));
        floor_game_objects_.push_back(f2);
        bossPatroller = new PatrollerEnemy(glm::vec3(60.0f, -3.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_crab], f2, true);
        enemy_game_objects_.push_back(bossPatroller);
        PopulateFloor(f2, 2, 0, 0, 5);

        // Floor 3 - heal station after boss 1
        FloorGameObject* f3 = new FloorGameObject(glm::vec3(86.0f, -2.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f3);
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(84.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(85.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(86.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(87.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(88.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));

        // Floor 4 - first encounter with turret enemy (jellyfish)
        FloorGameObject* f4 = new FloorGameObject(glm::vec3(100.0f, -3.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(8, 1));
        floor_game_objects_.push_back(f4);
        PopulateFloor(f4, 2, 1, 0, 0);

        // Floor 5 - mixed enemies before exit area
        FloorGameObject* f5 = new FloorGameObject(glm::vec3(135.0f, -3.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(14, 1));
        floor_game_objects_.push_back(f5);
        PopulateFloor(f5, 0, 2, 1, 5);

        // Floor 6 - exit platform (gatekeeper blob blocks passage until both shells collected)
        FloorGameObject* f6 = new FloorGameObject(glm::vec3(198.0f, -4.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(44, 1));
        floor_game_objects_.push_back(f6);

        // Upper platforms: vertical climb toward boss 2 and boss 3

        // Floor 7 - first step up from ground level
        FloorGameObject* f7 = new FloorGameObject(glm::vec3(115.0f, 2.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f7);
        PopulateFloor(f7, 1, 1, 0, 0);

        // Floor 8 - stepping stone upward
        FloorGameObject* f8 = new FloorGameObject(glm::vec3(105.0f, 7.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f8);

        // Floor 9 - first dasher (shrimp) encounter, leads toward boss 3
        FloorGameObject* f9 = new FloorGameObject(glm::vec3(88.0f, 12.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(8, 1));
        floor_game_objects_.push_back(f9);
        PopulateFloor(f9, 0, 0, 1, 3);

        // Floor 10 - heal station before boss 2 (jellyfish)
        FloorGameObject* f10 = new FloorGameObject(glm::vec3(105.0f, 18.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f10);
        PopulateFloor(f10, 0, 0, 0, 3);
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(104.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(105.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(106.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));

        // Floor 11 - boss 2 (turret jellyfish), drops sacred shell 1
        FloorGameObject* f11 = new FloorGameObject(glm::vec3(125.0f, 22.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(12, 1));
        floor_game_objects_.push_back(f11);
        bossTurret = new TurretEnemy(glm::vec3(125.0f, 28.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_jellyfish], f11, true);
        enemy_game_objects_.push_back(bossTurret);
        PopulateFloor(f11, 3, 0, 0, 0);

        // Floor 12 - mixed enemies on the path toward boss 3
        FloorGameObject* f12 = new FloorGameObject(glm::vec3(65.0f, 17.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(10, 1));
        floor_game_objects_.push_back(f12);
        PopulateFloor(f12, 2, 1, 0, 5);

        // Floor 12.5 - heal station before boss 3
        FloorGameObject* f12_2 = new FloorGameObject(glm::vec3(43.0f, 20.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(4, 1));
        floor_game_objects_.push_back(f12_2);
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(42.0f, 22.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(43.0f, 22.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));
        collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(44.0f, 22.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true));

        // Floor 13 - boss 3 (dasher shrimp), drops sacred shell 2
        FloorGameObject* f13 = new FloorGameObject(glm::vec3(17.0f, 24.0f, 0), sprite_, &sprite_shader_, tex_[tex_tileable_grass], glm::vec2(16, 1));
        floor_game_objects_.push_back(f13);
        bossDasher = new DasherEnemy(glm::vec3(17.0f, 30.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_shrimp], f13, true);
        enemy_game_objects_.push_back(bossDasher);


        // ============ NPCs and dialogue text sprites ============
        // Friendly NPC fish on starting platform - shows intro text when player is nearby
        friendlyFish = new GameObject(glm::vec3(-3.0f, -2.4f, 0.0f), sprite_, &sprite_shader_, tex_[tex_fish1], glm::vec2(1.5f));
        introText = new GameObject(glm::vec3(-3.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_text_intro], glm::vec2(16, 2));

        // "You learned to throw fish!" - shown briefly after boss 1 dies
        gunUnlockText = new GameObject(glm::vec3(60, 4, 0), sprite_, &sprite_shader_, tex_[tex_gunUnlockText], glm::vec2(16, 1.5f));

        // Gatekeeper blob - blocks exit until both sacred shells are collected
        gatekeeper = new GameObject(glm::vec3(184, 0, 0), sprite_, &sprite_shader_, tex_[tex_blob], glm::vec2(10.0f));
        noShellsText = new GameObject(glm::vec3(184, 6, 0), sprite_, &sprite_shader_, tex_[tex_noShellsText], glm::vec2(12, 1.5f));

        // Win text - rendered above player after passing gatekeeper with both shells
        winText = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_winText], glm::vec2(20, 1.5f));

        // Mid-level blob - blocks upward path until player has visited the exit gatekeeper
        midGatekeeper = new GameObject(glm::vec3(105, 9.5f, 0), sprite_, &sprite_shader_, tex_[tex_blob], glm::vec2(6.0f));

        // Thought bubble text - shown above player after encountering gatekeeper
        passageThoughtText = new GameObject(glm::vec3(0), sprite_, &sprite_shader_, tex_[tex_passageThoughtText], glm::vec2(9, 1));


        // ============ 3-layer parallax background ============
        // Each layer scrolls at a different rate relative to camera for depth effect
        float n = 2;
        float background_scale_x = 32 * n * 10;
        float background_scale_y = 18 * n;
        glm::vec2 background_scale(background_scale_x, background_scale_y);

        background_far = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_bg_far]);
        background_far->SetScale(background_scale);
        background_far->SetTilingFactor(glm::vec2(10, 1));

        background_mid = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_bg_mid]);
        background_mid->SetScale(background_scale);
        background_mid->SetTilingFactor(glm::vec2(10, 1));

        background_front = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_bg_front]);
        background_front->SetScale(background_scale);
        background_front->SetTilingFactor(glm::vec2(10, 1));
    }


    // Free all heap-allocated game objects
    void Game::DestroyGameWorld(void)
    {
        delete player;
        delete player_hand;
        delete background_far;
        delete background_mid;
        delete background_front;
        delete health_icon;
        delete ammo_icon;
        delete score_icon;
        delete shell1_icon;
        delete shell2_icon;
        delete friendlyFish;
        delete introText;
        delete gatekeeper;
        delete midGatekeeper;
        delete gunUnlockText;
        delete noShellsText;
        delete winText;
        delete passageThoughtText;

        // Vectors handle bosses too since they were pushed into enemy_game_objects_
        FreeVector(enemy_game_objects_);
        FreeVector(floor_game_objects_);
        FreeVector(collectible_game_objects_);
        FreeVector(bullet_game_objects_);
        FreeVector(melee_game_objects_);
        FreeVector(explosions_);
        FreeVector(dirt_);
    }


    void Game::HandleControls(double delta_time)
    {
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window_, true);

        glm::vec3 curpos = player->GetPosition();
        float speed = delta_time * 50.0f;
        glm::vec3 impulse(0.0f);

        // WASD / Arrow key movement
        if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS ||
            glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            impulse.x += speed;
            player->SetDirection(1);
        }
        if (glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS ||
            glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            impulse.x -= speed;
            player->SetDirection(-1);
        }
        // Jump - only allowed when on ground
        if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS ||
            glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            if (player->OnGround()) {
                impulse.y += speed * 30;
                player->MarkInAir();
            }
        }
        if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS ||
            glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            impulse.y -= speed;
        }

        if (impulse.x != 0.0f || impulse.y != 0.0f) {
            player->AddVelocity(impulse);
        }


        // Melee attack (space) - slash in front of player, scaled up if melee boost is active
        if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (meleeCdTimer_.Finished()) {
                MeleeGameObject* m = new MeleeGameObject(sprite_, &sprite_shader_, tex_[tex_slash], player, 1.2, glm::vec2(2));
                if (!player->GetMeleeBoostTimer().Finished()) m->SetScale(m->GetScale() * 2.0f);
                melee_game_objects_.push_back(m);
                meleeCdTimer_.Start(meleeCdSec_);
            }
        }

        // Ranged attack (left click) - fires a squid projectile toward the mouse cursor
        // Only available after gun is unlocked by defeating boss 1 (Patroller Crab)
        if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT)) {
            if (shotCdTimer_.Finished() && player->GetAmmo() > 0 && player->GetGunUnlocked()) {
                glm::vec2 mousePos = glm::vec2(-999, 999);
                GetMousePosition(mousePos);
                float aimAngle = 0;
                glm::vec3 aimDir(1, 0, 0);
                // Calculate aim direction from player to mouse in world coordinates
                if (mousePos.x != -999 && mousePos.y != -999) {
                    glm::vec2 toMouse = mousePos - glm::vec2(curpos.x, curpos.y);
                    if (glm::length(toMouse) > 0.0001f) {
                        aimAngle = atan2(toMouse.y, toMouse.x);
                        aimDir = glm::vec3(cos(aimAngle), sin(aimAngle), 0);
                    }
                }
                // Spawn bullet slightly offset from player in aim direction
                BulletGameObject* bullet = new BulletGameObject(curpos + aimDir * glm::vec3(1.1), sprite_, &sprite_shader_, tex_[tex_squid], true, glm::vec2(0.4, 0.4), aimAngle - glm::radians(135.0f));
                bullet->SetVelocity(aimDir * glm::vec3(35));
                bullet_game_objects_.push_back(bullet);
                shotCdTimer_.Start(shotCdSec_);
                player->Shoot();
            }
        }

        // Toggle wireframe debug mode
        if (glfwGetKey(window_, GLFW_KEY_SLASH) == GLFW_PRESS) {
            debug_mode_ = !debug_mode_;
        }
    }


    void Game::Update(double delta_time)
    {
        // Spawn dirt particles at player's feet when moving on ground
        if (player->OnGround() && glm::abs(player->GetVelocity().x) > 0.5f) {
            if (dirtCdTimer_.Finished()) {
                SpawnDirt(player->GetPosition() + glm::vec3(-0.3f * player->GetDirection(), -0.8f, 0));
                dirtCdTimer_.Start(0.3);
            }
        }

        // Update all game objects
        player->Update(delta_time);
        HandleEnemyUpdates(delta_time);
        UpdateVector(collectible_game_objects_, delta_time);
        UpdateVector(bullet_game_objects_, delta_time);
        UpdateVector(melee_game_objects_, delta_time);

        HandleBackground();
        HandleCollisions();

        // ============ Boss kill rewards ============
        // Boss 1 death unlocks the ranged weapon
        if (!bossPatroller->IsAlive() && !player->GetGunUnlocked()) {
            player->SetGunUnlocked();
            gunUnlockTextTimer.Start(8);
        }
        // Boss 2 and 3 deaths grant sacred shells needed to pass the gatekeeper
        if (!bossTurret->IsAlive())
            player->SetShell1Owned();
        if (!bossDasher->IsAlive())
            player->SetShell2Owned();

        // ============ Invisible walls ============
        // Block exit until both shells are collected
        if (!player->GetShell1Owned() || !player->GetShell2Owned()) {
            if (player->GetPosition().x > 177.0f) {
                player->SetPosition(glm::vec3(177.0f, player->GetPosition().y, 0));
                player->SetVelocity(glm::vec3(0, player->GetVelocity().y, 0));
            }
        }

        // Block upward path until player has visited the exit gatekeeper first
        if (!passage) {
            if (player->GetPosition().x < 109.2f && player->GetPosition().x > 100.0f && player->GetPosition().y > 7.5f) {
                player->SetPosition(glm::vec3(109.2f, player->GetPosition().y, 0));
                player->SetVelocity(glm::vec3(0, player->GetVelocity().y, 0));
            }
        }

        // Teleports player to starting position if fell off the map
        ResetPlayerPositionOnFall();

        // Player died or timer ran out - trigger game over
        if (!player->IsAlive() || gameOverTimer_.Finished())
            gameOver_ = true;

        // Player passed the gatekeeper with both shells - start win countdown
        if (player->GetPosition().x > 200 && !gameOver_) {
            winTimer.Start(10);
            gameOver_ = true;
        }

        // Close window after win timer expires (gives player 10 sec to see victory text)
        if (gameOver_ && winTimer.Finished())
            glfwSetWindowShouldClose(window_, true);

        // Clean up dead objects at end of frame
        RemoveMarkedFromVector(enemy_game_objects_);
        RemoveMarkedFromVector(collectible_game_objects_);
        RemoveMarkedFromVector(bullet_game_objects_);
        RemoveMarkedFromVector(melee_game_objects_);
        RemoveMarkedFromVector(explosions_);
        RemoveMarkedFromVector(dirt_);

        current_time_ += delta_time;
    }


    void Game::Render(void) {
        // Clear background
        glClearColor(viewport_background_color_g.r, viewport_background_color_g.g, viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Scale rendering to maintain correct aspect ratio regardless of window size
        int width, height;
        glfwGetWindowSize(window_, &width, &height);
        glm::mat4 window_scale_matrix(1.0f);
        if (width > height) {
            float aspect_ratio = ((float)width) / ((float)height);
            window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / aspect_ratio, 1.0f, 1.0f));
        }
        else {
            float aspect_ratio = ((float)height) / ((float)width);
            window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f / aspect_ratio, 1.0f));
        }

        // Nightfall darkening - gradually increases darkness from darknessStart_ to darknessEnd_
        if (darkening_) {
            if (current_time_ > darknessStart_) {
                currentDarkness_ = glm::clamp((float)(current_time_ - darknessStart_) / (darknessEnd_ - darknessStart_), 0.0f, 0.6f);
            }
        }

        // Camera follows player with offset, zoomed out to show surrounding area
        glm::vec3 player_pos = player->GetPosition();
        glm::mat4 camera_translation_matrix = glm::translate(glm::mat4(1.0f), -glm::vec3(player_pos.x + camera_offset_.x, player_pos.y + camera_offset_.y, player_pos.z));
        glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom_, camera_zoom_, camera_zoom_));
        glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_translation_matrix;

        if (debug_mode_) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Render HUD and melee slashes
        RenderHUD(view_matrix, current_time_);
        RenderVector(melee_game_objects_, view_matrix, current_time_);

        // Calculate aim angle from player to mouse cursor
        glm::vec2 mousePos = glm::vec2(-999, 999);
        GetMousePosition(mousePos);
        if (mousePos.x != -999 && mousePos.y != -999) {
            glm::vec2 toMouse = mousePos - glm::vec2(player->GetPosition().x, player->GetPosition().y);
            if (glm::length(toMouse) > 0.0001f)
                aimAngle_ = atan2(toMouse.y, toMouse.x);
        }

        // Hierarchical transform: hand orbits the player at aim angle
        float radius = 1;
        glm::mat4 hand_offset = glm::translate(glm::mat4(1.0f), glm::vec3(radius * cos(aimAngle_), radius * sin(aimAngle_), 0.0f));

        // Render hand relative to player's position (parent * offset)
        player_hand->Render(view_matrix, current_time_, player->GetTranslationMatrix(), hand_offset);
        player->Render(view_matrix, current_time_);

        // ============ NPC and dialogue rendering ============
        // Friendly fish NPC - shows intro text when player is nearby
        friendlyFish->Render(view_matrix, current_time_);
        if (CircleCollision(player, 0.4, friendlyFish, 1))
            introText->Render(view_matrix, current_time_);

        // Mid-level blob - hidden after player has visited the exit gatekeeper
        if (!passage)
            midGatekeeper->Render(view_matrix, current_time_);

        // Win text - follows player after passing gatekeeper with both shells
        if (gameOver_)
            winText->Render(view_matrix, current_time_, player->GetTranslationMatrix(), glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 5.0f, 0.0f)));

        // Thought bubble - shown above player after first encountering the gatekeeper
        if (!passageThoughtTimer.Finished() && passage)
            passageThoughtText->Render(view_matrix, current_time_, player->GetTranslationMatrix(), glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));

        // Exit gatekeeper - shows "You shell not pass!" when player is nearby without both shells
        gatekeeper->Render(view_matrix, current_time_);
        if (CircleCollision(player, 0.4, gatekeeper, 9) && (!player->GetShell1Owned() || !player->GetShell2Owned())) {
            noShellsText->Render(view_matrix, current_time_);
            // First encounter unlocks the upward path and triggers thought bubble
            if (!passage) {
                passage = true;
                passageThoughtTimer.Start(5);
            }
        }

        // Gun unlock popup - shown briefly after defeating boss 1
        if (player->GetGunUnlocked() && !gunUnlockTextTimer.Finished())
            gunUnlockText->Render(view_matrix, current_time_);

        // Render game world objects
        RenderVector(enemy_game_objects_, view_matrix, current_time_);
        RenderVector(collectible_game_objects_, view_matrix, current_time_);
        RenderVector(bullet_game_objects_, view_matrix, current_time_);
        RenderVector(floor_game_objects_, view_matrix, current_time_);

        // Parallax backgrounds - rendered back to front
        background_front->Render(view_matrix, current_time_);
        background_mid->Render(view_matrix, current_time_);
        background_far->Render(view_matrix, current_time_);

        if (debug_mode_) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Particle effects rendered last (on top of everything)
        RenderVector(explosions_, view_matrix, current_time_);
        RenderVector(dirt_, view_matrix, current_time_);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }


    void Game::MainLoop(void)
    {
        // Loop while the user did not close the window
        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(window_)){

            // Calculate delta time
            double current_time = glfwGetTime();
            double delta_time = current_time - last_time;
            last_time = current_time;

            // Update window events like input handling
            glfwPollEvents();

            // Handle user input
            HandleControls(delta_time);

            // Update all the game objects
            Update(delta_time);

            // Render all the game objects
            Render();

            // Push buffer drawn in the background onto the display
            glfwSwapBuffers(window_);
        }
    }


    // Converts mouse pixel coordinates to world space, accounting for aspect ratio, zoom, and camera offset
    bool Game::GetMousePosition(glm::vec2& mouse) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        int width, height;
        glfwGetWindowSize(window_, &width, &height);

        if (x < 0 || x > width || y < 0 || y > height) {
            return false;
        }

        // Convert pixel coordinates to normalized device coordinates, adjusted for aspect ratio and zoom
        if (width > height) {
            float aspect_ratio = ((float)width) / ((float)height);
            mouse.x = ((2.0f * x - width) * aspect_ratio) / (width * camera_zoom_);
            mouse.y = (-2.0f * y + height) / (height * camera_zoom_);
        }
        else {
            float aspect_ratio = height / width;
            mouse.x = (2.0f * x - width) / (width * camera_zoom_);
            mouse.y = ((-2.0f * y + height) * aspect_ratio) / (height * camera_zoom_);
        }

        // Offset by camera position to get final world coordinates
        mouse.x += player->GetPosition().x + camera_offset_.x;
        mouse.y += player->GetPosition().y + camera_offset_.y;
        return true;
    }


    Game::Game(void)
    {
        // Don't do work in the constructor, leave it for the Init() function
    }


    void Game::Init(void)
    {
        debug_mode_ = false;
        darkening_ = true;
        gameOver_ = false;
        passage = false;

        // Initialize the window management library (GLFW)
        if (!glfwInit()) {
            throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
        }

        // Set whether window can be resized, start maximized
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

        // Create a window and its OpenGL context
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
        if (!window_) {
            glfwTerminate();
            throw(std::runtime_error(std::string("Could not create window")));
        }

        // Make the window's OpenGL context the current one
        glfwMakeContextCurrent(window_);

        // Initialize the GLEW library to access OpenGL extensions
        // Need to do it after initializing an OpenGL context
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char*)glewGetErrorString(err))));
        }

        // Set event callbacks
        glfwSetFramebufferSizeCallback(window_, ResizeCallback);

        // Initialize sprite geometry and shader (used by all game objects)
        sprite_ = new Sprite();
        sprite_->CreateGeometry();
        sprite_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/sprite_fragment_shader.glsl")).c_str());

        // Explosion particle system - 400 particles, red-orange burst effect
        explosion_shader_.Init((resources_directory_g + std::string("/explosion_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/explosion_fragment_shader.glsl")).c_str());
        explosion_geom_ = new Particles();
        explosion_geom_->CreateGeometry(400, EXPLOSION);

        // Dirt particle system - 50 particles, brown puffs at player's feet
        dirt_shader_.Init((resources_directory_g + std::string("/dirt_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/dirt_fragment_shader.glsl")).c_str());
        dirt_geom_ = new Particles();
        dirt_geom_->CreateGeometry(50, DIRT);

        // Initialize time
        current_time_ = 0.0;

        // Camera setup - zoomed out to show the game world
        camera_zoom_ = 0.1f;
        camera_offset_ = glm::vec2(4, 3);

        // Seed random number generator for enemy placement and loot drops
        srand(time(NULL));

        // Player attack cooldowns (seconds)
        shotCdSec_ = 0.5f;
        meleeCdSec_ = 0.5f;
        aimAngle_ = 0;

        // Nightfall timer - 5 min limit, darkening starts at 20% and maxes at 80%
        timeLimit_ = 300.0f;
        darknessStart_ = timeLimit_ * 0.2f;
        darknessEnd_ = timeLimit_ * 0.8f;
        currentDarkness_ = 0.0f;
        if (darkening_)
            gameOverTimer_.Start(timeLimit_);

        passage = false;
    }


    Game::~Game()
    {
        // Free memory used by game world
        DestroyGameWorld();

        // Free rendering resources
        delete sprite_;

        // Close window
        glfwDestroyWindow(window_);
        glfwTerminate();
    }


    void Game::ResizeCallback(GLFWwindow* window, int width, int height)
    {

        // Set OpenGL viewport based on framebuffer width and height
        glViewport(0, 0, width, height);
    }


    void Game::SetTexture(GLuint w, const char *fname)
    {
        // Bind texture buffer
        glBindTexture(GL_TEXTURE_2D, w);

        // Load texture from a file to the buffer
        int width, height;
        unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
        if (!image){
            std::cout << "Cannot load texture " << fname << std::endl;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Texture Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


    void Game::LoadTextures(std::vector<std::string> &textures)
    {
        // Allocate a buffer for all texture references
        int num_textures = textures.size();
        tex_ = new GLuint[num_textures];
        glGenTextures(num_textures, tex_);
        // Load each texture
        for (int i = 0; i < num_textures; i++){
            SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
        }
        // Set first texture in the array as default
        glBindTexture(GL_TEXTURE_2D, tex_[0]);
    }


    // Handles per-frame enemy behavior that requires access to game-level state
    // (bullet spawning, player position). Enemy subclasses don't have access to
    // bullet_game_objects_ or player, so their attack logic lives here rather
    // than inside each enemy's Update().
    void Game::HandleEnemyUpdates(double delta_time) {
        for (EnemyGameObject* enemy : enemy_game_objects_) {

            // --- Turret (jellyfish) behavior ---
            // Stationary enemy that shoots projectiles at the player when aggro
            TurretEnemy* turret = dynamic_cast<TurretEnemy*>(enemy);
            if (turret) {
                // Aggro when player is on the same floor tile as the turret's parent floor
                turret->SetState(player->GetCurrentFloor() == turret->GetParentFloor() ? AGGRO : PATROL);

                if (turret->GetState() == AGGRO && turret->GetCdTimer().Finished()) {
                    // Calculate aim direction from turret toward player
                    glm::vec3 dir = player->GetPosition() - turret->GetPosition();
                    float angle = atan2(dir.y, dir.x);
                    glm::vec3 aimDir(cos(angle), sin(angle), 0);

                    // Spawn a squid projectile offset slightly in the aim direction to avoid self-collision
                    BulletGameObject* bullet = new BulletGameObject(turret->GetPosition() + aimDir * 1.1f, sprite_, &sprite_shader_, tex_[tex_squid], false, glm::vec2(0.4f, 0.4f), angle - glm::radians(135.0f));
                    bullet->SetVelocity(aimDir * 15.0f);
                    bullet_game_objects_.push_back(bullet);
                    turret->ShotDone(); // Reset turret's shot cooldown
                }
            }

            // --- Dasher (shrimp) behavior ---
            // Patrols back and forth; dashes toward the player when aggro and off cooldown
            DasherEnemy* dasher = dynamic_cast<DasherEnemy*>(enemy);
            if (dasher) {
                // Same floor-based aggro check as turret
                dasher->SetState(player->GetCurrentFloor() == dasher->GetParentFloor() ? AGGRO : PATROL);

                // Only dash when on the ground (prevents mid-air chain dashes)
                if (dasher->GetState() == AGGRO && dasher->GetCdTimer().Finished() && dasher->OnGround()) {
                    // Calculate aim direction from dasher toward player
                    glm::vec3 dir = player->GetPosition() - dasher->GetPosition();
                    float angle = atan2(dir.y, dir.x);
                    glm::vec3 aimDir(cos(angle), sin(angle), 0);

                    dasher->SetDirection(dir.x < 0 ? -1 : 1);     // Face the player
                    dasher->StartDashTimer(0.6);                  // Lock into dash for 0.6 seconds
                    dasher->SetVelocity(glm::vec3(aimDir.x * 14.0f, aimDir.y * 12.0f, 0.0f)); // Launch toward player

                    // Nudge slightly upward to prevent immediate floor re-collision from canceling the dash
                    dasher->SetPosition(glm::vec3(dasher->GetPosition().x, dasher->GetPosition().y + 0.1, 0.0f));
                    dasher->MarkInAir();
                    dasher->DashDone();  // Reset dash cooldown
                }
            }

            enemy->Update(delta_time);
        }
    }


    void Game::HandleCollisions() {
        // Player floor collision
        player->SetWasOnGround(false);
        for (FloorGameObject* floor : floor_game_objects_) {
            CollisionResult res = CircleAABBCollision(player, floor, player->GetScale().y * 0.4f);
            if (res.onGround) {
                player->SetWasOnGround(true);
                player->SetCurrentFloor(floor);
            }
        }
        if (player->WasOnGround()) player->MarkOnGround();
        else player->MarkInAir();

        // Enemies floor collision
        for (EnemyGameObject* enemy : enemy_game_objects_) {
            for (FloorGameObject* floor : floor_game_objects_) {
                CollisionResult res = CircleAABBCollision(enemy, floor, enemy->GetScale().y * 0.4f);
                if (res.onGround) enemy->MarkOnGround();
            }
        }

        // Collectibles floor collision
        for (CollectibleGameObject* col : collectible_game_objects_) {
            for (FloorGameObject* floor : floor_game_objects_) {
                CollisionResult res = CircleAABBCollision(col, floor, 0.3f);
                if (res.onGround) col->MarkOnGround();
            }
        }

        // ============ Enemy vs Player/Weapons collisions ============
        for (EnemyGameObject* enemy : enemy_game_objects_) {
            bool kill = false;

            // Body-to-body: enemy touching player deals 1 damage (contact damage)
            if (CircleCollision(player, player->GetScale().y * 0.4f, enemy, enemy->GetScale().y * 0.4f)) {
                player->GetHit(1);
            }

            // Bullet-to-enemy: ray cast from bullet's previous to current position
            // catches fast-moving projectiles that would skip past enemies in a single frame.
            // Only player-owned bullets damage enemies.
            for (BulletGameObject* bullet : bullet_game_objects_)
                if (RayToCircleCollision(bullet->GetPrevPos(), bullet->GetPosition(), enemy->GetPosition(), enemy->GetScale().y * 0.55f) && bullet->IsPlayerBullet() && !bullet->IsMarkedForRemoval()) {
                    bullet->MarkForRemoval();
                    enemy->GetHit(1);  // Ranged does 1 damage per hit
                    if (!enemy->IsAlive()) {
                        kill = true;
                    }
                }

            // Melee slash-to-enemy: circle-circle overlap test.
            // HasHit/RegisterHit prevents the same slash from damaging an enemy twice
            // (slash persists for a few frames).
            for (MeleeGameObject* melee : melee_game_objects_) {
                if (CircleCollision(melee, melee->GetScale().y * 0.2f, enemy, enemy->GetScale().y * 0.4f) && !melee->HasHit(enemy)) {
                    melee->RegisterHit(enemy);
                    enemy->GetHit(3);  // Melee does 3 damage - rewards close-range risk
                    if (!enemy->IsAlive()) {
                        kill = true;
                    }
                }
            }

            // ---- Enemy death: explosion + loot drop + score ----
            if (kill) {
                SpawnExplosion(enemy->GetPosition());

                // Loot table (roll 0-99):
                //   0-29  Å® shell   (30% chance, score item)
                //  30-49  Å® health  (20% chance)
                //  50-69  Å® ammo    (20% chance)
                //  70-79  Å® melee boost flower (10% chance, timed power-up)
                //  80-99  Å® nothing (20% chance)
                // All drops spawn with gravity enabled so they fall to the nearest floor.
                int roll = rand() % 100;
                if (roll < 30) {
                    CollectibleGameObject* col = new CollectibleGameObject(enemy->GetPosition(), sprite_, &sprite_shader_, tex_[tex_shell], CollectibleType::Shell, true, glm::vec2(0.6));
                    collectible_game_objects_.push_back(col);
                }
                else if (roll < 50) {
                    CollectibleGameObject* col = new CollectibleGameObject(enemy->GetPosition(), sprite_, &sprite_shader_, tex_[tex_clownfish], CollectibleType::Health, true, glm::vec2(0.6));
                    collectible_game_objects_.push_back(col);
                }
                else if (roll < 70) {
                    CollectibleGameObject* col = new CollectibleGameObject(enemy->GetPosition(), sprite_, &sprite_shader_, tex_[tex_squid], CollectibleType::Ammo, true, glm::vec2(0.6));
                    collectible_game_objects_.push_back(col);
                }
                else if (roll < 80) {
                    CollectibleGameObject* col = new CollectibleGameObject(enemy->GetPosition(), sprite_, &sprite_shader_, tex_[tex_flower], CollectibleType::Melee, true, glm::vec2(0.7));
                    collectible_game_objects_.push_back(col);
                }
                // else: no drop (20% chance)

                enemy->MarkForRemoval();
                player->AddScore(5);  // Flat 5 points per kill, on top of any shell pickups
            }
        }


        // ============ Bullet cleanup ============
        for (BulletGameObject* bullet : bullet_game_objects_) {
            // Destroy bullets that hit any floor tile
            for (FloorGameObject* floor : floor_game_objects_) {
                CollisionResult res = CircleAABBCollision(bullet, floor, 0.05f);
                if (res.collided) bullet->MarkForRemoval();
            }

            // Enemy bullets hitting the player - same ray-cast approach as player bullets above,
            // but only for non-player-owned bullets
            if (RayToCircleCollision(bullet->GetPrevPos(), bullet->GetPosition(), player->GetPosition(), player->GetScale().y * 0.55f) && !bullet->IsPlayerBullet() && !bullet->IsMarkedForRemoval()) {
                player->GetHit(1);
                bullet->MarkForRemoval();
            }
        }


        // ============ Collectible pickup ============
        // Circle-circle overlap between player and each collectible
        for (CollectibleGameObject* col : collectible_game_objects_) {
            if (!col->IsMarkedForRemoval() && CircleCollision(player, 0.5f, col, 0.8f)) {
                switch (col->GetType()) {
                case CollectibleType::Shell:  player->AddScore(col->GetValue()); break;                   // +10 score
                case CollectibleType::Health: player->Heal(col->GetValue()); break;                       // +1 HP
                case CollectibleType::Ammo:   player->AddAmmo(col->GetValue()); break;                    // +3 ammo
                case CollectibleType::Melee:  player->GetMeleeBoostTimer().Start(col->GetValue()); break; // Timed melee range boost
                }
                col->MarkForRemoval();
            }
        }
    }


    // Positions the three parallax background layers each frame.
    // Each layer scrolls at a different rate relative to the camera -
    // far layer moves slowest, front layer moves fastest - creating depth.
    void Game::HandleBackground() {
        glm::vec3 cam = player->GetPosition() + glm::vec3(camera_offset_, 0.0f);
        float bgOffsetY = 6.0f;  // Vertical offset to center backgrounds above the ground

        // Vertical parallax: each layer shifts down slightly as the player climbs,
        // clamped so backgrounds don't scroll out of view at extreme heights
        float parallaxY_far = glm::clamp(player->GetPosition().y * 0.03f, -1.2f, 12.2f);
        float parallaxY_mid = glm::clamp(player->GetPosition().y * 0.06f, -1.2f, 12.2f);
        float parallaxY_front = glm::clamp(player->GetPosition().y * 0.09f, -1.2f, 12.2f);

        // Horizontal parallax: subtract a fraction of player X from camera X
        // so each layer lags behind the camera by a different amount.
        // Multipliers: far=0.003, mid=0.015, front=0.035 (higher = more movement = closer feel)
        background_far->SetPosition(glm::vec3(cam.x - player->GetPosition().x * 0.003f, cam.y + bgOffsetY - parallaxY_far, 0.0f));
        background_mid->SetPosition(glm::vec3(cam.x - player->GetPosition().x * 0.015f, cam.y + bgOffsetY - parallaxY_mid, 0.0f));
        background_front->SetPosition(glm::vec3(cam.x - player->GetPosition().x * 0.035f, cam.y + bgOffsetY - parallaxY_front, 0.0f));
    }


    // Tests if a line segment (P0 Å® P1) intersects a circle centered at C.
    // Used for bullet collision - ray-casting between the bullet's previous and
    // current position catches hits even when the bullet moves fast enough to
    // skip past an enemy in a single frame (tunneling prevention).
    bool Game::RayToCircleCollision(const glm::vec3& P0 /*bullet prev pos*/, const glm::vec3& P1 /*bullet cur pos*/, const glm::vec3& C /*circle center*/, float radius) {
        glm::vec3 d = P1 - P0;                                  // Segment direction vector
        glm::vec3 f = C - P0;                                   // Vector from segment start to circle center
        float t = glm::dot(f, d) / glm::dot(d, d);              // Project circle center onto segment (parametric t)
        t = glm::clamp(t, 0.0f, 1.0f);                          // Clamp to segment bounds [P0, P1]
        glm::vec3 closest = P0 + t * d;                         // Nearest point on segment to circle center
        float distSquared = glm::dot(closest - C, closest - C); // Squared distance to avoid sqrt
        return distSquared <= radius * radius;                  // Hit if nearest point is inside the circle
    }


    // Simple circle-circle overlap test - used for body collisions, melee hits, and pickups
    bool Game::CircleCollision(GameObject* a, float r1, GameObject* b, float r2) {
        return glm::distance(a->GetPosition(), b->GetPosition()) < r1 + r2;
    }


    // Circle vs axis-aligned bounding box collision with resolution.
    // Used for player/enemy/collectible vs floor tiles. Resolves overlap by pushing
    // the circle out and canceling velocity into the surface (so objects stand on
    // floors and slide along walls instead of passing through).
    CollisionResult Game::CircleAABBCollision(GameObject* circle, FloorGameObject* floor, float radius) {
        CollisionResult result;

        glm::vec3 pos = circle->GetPosition();
        glm::vec3 vel = circle->GetVelocity();
        glm::vec3 floor_pos = floor->GetPosition();

        float floorHalfW = floor->GetScale().x * 0.5f;
        float floorHalfH = floor->GetScale().y * 0.5f;

        // Find the closest point on the AABB to the circle's center
        float closestX = glm::clamp(pos.x, floor_pos.x - floorHalfW, floor_pos.x + floorHalfW);
        float closestY = glm::clamp(pos.y, floor_pos.y - floorHalfH, floor_pos.y + floorHalfH);

        // Vector from closest AABB point to circle center
        float dx = pos.x - closestX;
        float dy = pos.y - closestY;
        float distSq = dx * dx + dy * dy;

        // No collision if distance exceeds radius
        if (distSq >= radius * radius)
            return result;

        result.collided = true;
        float dist = std::sqrt(distSq);
        float nx, ny;  // Collision normal - direction to push the circle out

        if (dist < 0.0001f) {
            // Edge case: circle center is fully inside the AABB (closest point == center).
            // Can't compute a normal from the distance vector, so instead find which
            // AABB edge is nearest and push out along that axis.
            float pushLeft = pos.x - (floor_pos.x - floorHalfW);
            float pushRight = (floor_pos.x + floorHalfW) - pos.x;
            float pushDown = pos.y - (floor_pos.y - floorHalfH);
            float pushUp = (floor_pos.y + floorHalfH) - pos.y;

            float minPush = std::min({ pushLeft, pushRight, pushDown, pushUp });

            if (minPush == pushUp) {
                nx = 0; ny = 1;
                pos.y = floor_pos.y + floorHalfH + radius;
            }
            else if (minPush == pushDown) {
                nx = 0; ny = -1;
                pos.y = floor_pos.y - floorHalfH - radius;
            }
            else if (minPush == pushLeft) {
                nx = -1; ny = 0;
                pos.x = floor_pos.x - floorHalfW - radius;
            }
            else {
                nx = 1; ny = 0;
                pos.x = floor_pos.x + floorHalfW + radius;
            }
        }
        else {
            // Normal case: push circle out along the penetration vector
            // (direction from closest AABB point toward circle center)
            nx = dx / dist;
            ny = dy / dist;
            float penetration = radius - dist;
            pos.x += nx * penetration;
            pos.y += ny * penetration;
        }

        circle->SetPosition(glm::vec3(pos.x, pos.y, 0.0f));

        // Remove the velocity component going into the surface so the object
        // doesn't keep pushing through (e.g. gravity pulling player into floor)
        float velDotN = vel.x * nx + vel.y * ny;
        if (velDotN < 0) {
            circle->SetVelocity(glm::vec3(vel.x - velDotN * nx, vel.y - velDotN * ny, vel.z));
        }

        // Surface normal pointing mostly upward means the circle is resting on top
        if (ny > 0.5f) {
            result.onGround = true;
        }

        return result;
    }


    // Debug utility - prints player position and/or velocity to console
    void Game::DebugPrintPlayer(bool pos, bool vel) {
        if (pos) {
            glm::vec3 p = player->GetPosition();
            printf("Position: (%.2f, %.2f, %.2f)\n", p.x, p.y, p.z);
        }
        if (vel) {
            glm::vec3 v = player->GetVelocity();
            printf("Velocity: (%.2f, %.2f, %.2f)\n", v.x, v.y, v.z);
        }
    }


    // Safety net: if the player falls below y=-15 (off the map), reset them
    // back to spawn. Keeps a small downward velocity so gravity feels continuous.
    void Game::ResetPlayerPositionOnFall() {
        if (player->GetPosition().y < -15.0f) {
            player->SetVelocity(glm::vec3(0, -1, 0));
            player->SetPosition(player_start_pos_);
        }
    }


    // Draws the heads-up display anchored to the camera so it stays on screen.
    // Icons are positioned in world space offset from the camera center, since
    // there's no separate orthographic UI pass.
    void Game::RenderHUD(glm::mat4 view_matrix, double current_time) {
        glm::vec3 cam = player->GetPosition() + glm::vec3(camera_offset_, 0);

        // Top-left stack: icon + number for each stat
        // Health (clownfish icon)
        health_icon->SetPosition(cam + glm::vec3(-18, 9, 0));
        health_icon->Render(view_matrix, current_time_);
        RenderNumber(player->GetHealth(), cam + glm::vec3(-17, 9, 0), 1.0f, view_matrix);

        // Ammo (squid icon)
        ammo_icon->SetPosition(cam + glm::vec3(-18, 7.5, 0));
        ammo_icon->Render(view_matrix, current_time_);
        RenderNumber(player->GetAmmo(), cam + glm::vec3(-17, 7.5, 0), 1.0f, view_matrix);

        // Score (shell icon)
        score_icon->SetPosition(cam + glm::vec3(-18, 6, 0));
        score_icon->Render(view_matrix, current_time_);
        RenderNumber(player->GetScore(), cam + glm::vec3(-17, 6, 0), 1.0f, view_matrix);

        // Top-right: countdown timer (seconds remaining until nightfall game over)
        RenderNumber((int)gameOverTimer_.Remaining(), cam + glm::vec3(16, 9, 0), 1.0f, view_matrix);

        // Sacred shell indicators - only rendered once the player has collected each one
        shell1_icon->SetPosition(cam + glm::vec3(16, 7.5, 0));
        if (player->GetShell1Owned())
            shell1_icon->Render(view_matrix, current_time_);

        shell2_icon->SetPosition(cam + glm::vec3(18, 7.5, 0));
        if (player->GetShell2Owned())
            shell2_icon->Render(view_matrix, current_time_);
    }


    // Spawns a red-orange explosion burst at the given position (used on enemy death).
    // 400-particle system, lasts 1.5 seconds before being cleaned up.
    void Game::SpawnExplosion(glm::vec3 position) {
        ParticleSystem* explosion = new ParticleSystem(position, explosion_geom_, &explosion_shader_, tex_[tex_orb]);
        explosion->SetScale(0.1f);
        explosion->SetBirthTime(current_time_);
        explosion->StartLifetime(1.5);
        explosions_.push_back(explosion);
    }


    // Spawns brown dirt puffs at the given position (used at player's feet when running).
    // 50-particle system, lasts 0.5 seconds - smaller and shorter-lived than explosions.
    void Game::SpawnDirt(glm::vec3 position) {
        ParticleSystem* dirt = new ParticleSystem(position, dirt_geom_, &dirt_shader_, tex_[tex_orb]);
        dirt->SetScale(0.1f);
        dirt->SetBirthTime(current_time_);
        dirt->StartLifetime(0.5);
        dirt_.push_back(dirt);
    }


    // Renders an integer as a sequence of digit sprites using a spritesheet.
    // The numbers texture is a 320x32 strip containing digits 0-9 side by side,
    // so each digit occupies 10% of the texture width (tiling_factor 0.1).
    void Game::RenderNumber(int value, glm::vec3 position, float digitSize, glm::mat4 view_matrix) {
        std::string digits = std::to_string(value);

        sprite_shader_.Enable();
        sprite_shader_.SetUniform1f("darkness", currentDarkness_);
        sprite_shader_.SetUniformMat4("view_matrix", view_matrix);

        glBindTexture(GL_TEXTURE_2D, tex_[tex_numbers]);
        sprite_->SetGeometry(sprite_shader_.GetShaderProgram());

        for (int i = 0; i < digits.size(); i++) {
            int d = digits[i] - '0';

            // Select the correct digit from the spritesheet by offsetting UVs
            sprite_shader_.SetUniform2f("tiling_factor", glm::vec2(0.1f, 1.0f));   // Show 1/10th of the texture
            sprite_shader_.SetUniform2f("uv_offset", glm::vec2(d * 0.1f, 0.0f));   // Slide to the d-th digit

            // Place each digit side by side, offset by index * digitSize
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), position + glm::vec3(i * digitSize, 0, 0));
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(digitSize, digitSize, 1.0f));
            sprite_shader_.SetUniformMat4("transformation_matrix", translation * scale);

            glDrawElements(GL_TRIANGLES, sprite_->GetSize(), GL_UNSIGNED_INT, 0);
        }

        // Reset tiling/offset so subsequent renders use the full texture
        sprite_shader_.SetUniform2f("tiling_factor", glm::vec2(1.0f, 1.0f));
        sprite_shader_.SetUniform2f("uv_offset", glm::vec2(0.0f, 0.0f));
    }


    // Helper to populate a floor platform with enemies and collectibles.
    // Avoids hardcoding every spawn position - just pass in counts per type
    // and enemies are placed at random X positions within the floor bounds.
    void Game::PopulateFloor(FloorGameObject* floor, int patrollers, int turrets, int dashers, int shells) {
        // Spawn bounds: inset 1.75 units from each edge so enemies don't spawn at the very edge
        float left = floor->GetPosition().x - floor->GetScale().x * 0.5f + 1.75f;
        float right = floor->GetPosition().x + floor->GetScale().x * 0.5f - 1.75f;
        // Slightly above the floor surface so enemies don't spawn inside the tile
        float top = floor->GetPosition().y + floor->GetScale().y * 0.5f + 0.5f + 0.1f;

        // Ground enemies (patrollers and dashers) spawn on the floor surface
        int groundTotal = patrollers + dashers;
        if (groundTotal > 0) {
            for (int i = 0; i < patrollers; i++) {
                float x = left + static_cast<float>(rand()) / RAND_MAX * (right - left);
                enemy_game_objects_.push_back(new PatrollerEnemy(glm::vec3(x, top, 0), sprite_, &sprite_shader_, tex_[tex_crab], floor));
            }
            for (int i = 0; i < dashers; i++) {
                float x = left + static_cast<float>(rand()) / RAND_MAX * (right - left);
                enemy_game_objects_.push_back(new DasherEnemy(glm::vec3(x, top, 0), sprite_, &sprite_shader_, tex_[tex_shrimp], floor));
            }
        }

        // Turrets (jellyfish) spawn floating 4 units above the floor
        for (int i = 0; i < turrets; i++) {
            float x = left + static_cast<float>(rand()) / RAND_MAX * (right - left);
            enemy_game_objects_.push_back(new TurretEnemy(glm::vec3(x, top + 4.0f, 0), sprite_, &sprite_shader_, tex_[tex_jellyfish], floor));
        }

        // Shells are evenly spaced across the platform (not random) for consistent pickups,
        // floating 3 units above the floor with no gravity so they stay in place
        float shellSpacing = (right - left) / (shells + 1);
        float sx = left + shellSpacing;
        for (int i = 0; i < shells; i++) {
            collectible_game_objects_.push_back(new CollectibleGameObject(glm::vec3(sx, top + 3.0f, 0), sprite_, &sprite_shader_, tex_[tex_shell], CollectibleType::Shell, false));
            sx += shellSpacing;
        }
    }

} // namespace game