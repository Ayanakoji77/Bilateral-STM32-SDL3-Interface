
#include "game/bullet.h"

#include <SDL3/SDL_render.h>

#include <glm/fwd.hpp>

#include "game/gameobject.h"
Bullet::Bullet(SDL_Texture* atlasTexture, glm::vec2 position, float direction)
{
    this->texture = atlasTexture;
    this->tag = Tag::bullet;
    this->dynamic = true;
    this->collider = {4, 4, 10, 8};

    this->position = position + glm::vec2{18.0f, 15.0f};
    this->direction = direction;
    this->velocity = {bullet_velocity * direction, 0};
    animations.emplace_back(4, 0.6f, 0, 16, 16);
    animations.emplace_back(4, 0.6f, 1, 16, 16);
}

void Bullet::update(float deltaTime, const bool* keys)
{
    if (state == BulletState::Moving)
    {
        GameObject::update(deltaTime, keys);
    }
    if (!animations.empty())
    {
        animations[currentAnim].step(deltaTime);
    }
}

void Bullet::Render(SDL_Renderer* renderer, glm::vec2 offset)
{
    if (animations.empty())
    {
        return;
    }

    SDL_FRect src = animations[currentAnim].GetCurrentFrameSrc();

    SDL_FRect dst = {position.x + offset.x, position.y + offset.y, src.w, src.h};
    SDL_FlipMode flip = (direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated(renderer, texture, &src, &dst, 0, nullptr, flip);
}
