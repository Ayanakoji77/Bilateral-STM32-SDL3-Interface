#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <vector>

class GameObject
{
   public:
    glm::vec2 position{0.0f};
    glm::vec2 velocity{0.0f};

    SDL_FRect collider{0, 0, 32, 32};
    SDL_Texture* texture = nullptr;
    bool dynamic = false;

    enum class Tag
    {
        player,
        level,
        enemy
    } tag = Tag::level;
    virtual ~GameObject() = default;

    virtual void update(float deltaTime, const bool* keys)
    {
        if (dynamic)
        {
            position = position + (velocity * deltaTime);
        }
    }

    virtual void Render(SDL_Renderer* renderer)
    {
        if (!texture)
            return;

        SDL_FRect dst = {position.x, position.y, collider.w, collider.h};

        SDL_RenderTexture(renderer, texture, nullptr, &dst);
    }
};
