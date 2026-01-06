#pragma once

#include <SDL3/SDL_render.h>

#include <vector>

#include "core/animation.h"
#include "game/gameobject.h"
enum class BulletState
{
    Moving,
    Colliding,
    Inactive,
};
class Bullet : public GameObject
{
   private:
    BulletState state = BulletState::Moving;
    std::vector<Animation> animations;
    int currentAnim = 0;
    float direction = 0;
    float bullet_velocity = 70.0f;

   public:
    Bullet(SDL_Texture* atlasTexture, glm::vec2 position, float direction);
    void update(float deltaTime, const bool* keys) override;
    void Render(SDL_Renderer* renderer, glm::vec2 offset) override;
};
