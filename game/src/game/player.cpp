#include "player.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>

#include <cstdlib>

Player::Player(SDL_Texture* atlasTexture)
{
    // loading texture pointer
    // configure the player
    this->texture = atlasTexture;
    this->tag = Tag::player;
    this->dynamic = true;
    this->collider = {10, 18, 14, 14};

    animations.emplace_back(4, 1.2f, 2, 32, 32);  // idel
    animations.emplace_back(8, 0.6f, 3, 32, 32);  // run
}

void Player::update(float deltaTime, const bool* keys)
{
    float dirInput = 0;
    if (keys[SDL_SCANCODE_A])
        dirInput = -1;
    if (keys[SDL_SCANCODE_D])
        dirInput = 1;

    if (dirInput != 0)
    {
        state = PlayerState::Running;
        direction = dirInput;
        currentAnim = 1;
    }
    else if (std::abs(velocity.x) > 0.1f)
    {
        state = PlayerState::Idle;
        currentAnim = 0;
    }
    else
    {
        state = PlayerState::Idle;
        currentAnim = 0;
    }
    if (keys[SDL_SCANCODE_H] && this->isGrounded())
    {
        state = PlayerState::Jumping;
        this->velocity.y = this->jump_power;
        this->setGrounded(false);
    }

    // gravity
    velocity.y = velocity.y + (500.0f * deltaTime);
    // aceleration
    velocity.x = velocity.x + (dirInput * acceleration.x * deltaTime);

    if (std::abs(velocity.x) > maxSpeedX)
    {
        float sign = (velocity.x > 0) ? 1.0f : -1.0f;
        velocity.x = sign * maxSpeedX;
    }

    if (dirInput == 0 && velocity.x != 0)
    {
        const float factor = velocity.x > 0 ? -1.5f : 1.5f;
        float amount = factor * acceleration.x * deltaTime;

        // If the friction amount is larger than remaining speed, snap to 0
        if (std::abs(velocity.x) < std::abs(amount))
        {
            velocity.x = 0;
        }
        else
        {
            velocity.x += amount;
        }
    }
    if (!animations.empty())
    {
        animations[currentAnim].step(deltaTime);
    }

    GameObject::update(deltaTime, keys);
}

void Player::Render(SDL_Renderer* renderer)
{
    if (animations.empty())
        return;

    SDL_FRect src = animations[currentAnim].GetCurrentFrameSrc();

    SDL_FRect dst = {position.x, position.y, src.w, src.h};

    SDL_FlipMode flip = (direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated(renderer, texture, &src, &dst, 0, nullptr, flip);
}
