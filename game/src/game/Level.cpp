#include "Level.h"

#include <SDL3/SDL.h>

#include "core/resourceManager.h"
#include "player.h"

void Level::LoadMap(ResourceManager* res)
{
    /*
     * 1.Ground
     * 2.Panel
     * 3.Enemy
     * 4.Player
     * 5.Grass
     * 6.Brick
     */

    short map[MAP_ROWS][MAP_COLS] = {0};
    map[2][2] = 4;
    map[4][0] = map[4][1] = map[4][2] = map[3][3] = map[3][0] = 2;
    bool playerFound = false;
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            int type = map[r][c];
            if (type == 0)
                continue;

            float x = c * TILE_SIZE;
            float y = (320) - (MAP_ROWS - r) * TILE_SIZE;
            if (type == 4)
            {
                playerFound = true;
                auto p = std::make_unique<Player>(res->GetTexture("player"));
                p->position = {x, y};
                layers[LAYER_IDX_CHARACTERS].push_back(std::move(p));
            }
            else if (type == 2)
            {
                auto panel = std::make_unique<GameObject>();
                panel->texture = res->GetTexture("panel");
                panel->position = {x, y};
                panel->tag = GameObject::Tag::level;
                layers[LAYER_IDX_LEVEL].push_back(std::move(panel));
            }
        }
    }
    SDL_assert_release(playerFound && "No Player intialized check itup ");
}

void Level::Update(float deltaTime, const bool* keys)
{
    for (auto& layer : layers)
    {
        for (auto& obj : layer)
        {
            obj->update(deltaTime, keys);
        }
    }

    // Check Physics
    CheckCollisions(deltaTime);
}

void Level::Render(SDL_Renderer* renderer)
{
    for (int i = 0; i < this->TOTAL_LAYERS; i++)
    {
        for (auto& obj : layers[i])
        {
            obj->Render(renderer);
        }
    }
}

void Level::CheckCollisions(float deltaTime)
{
    for (auto& character : layers[LAYER_IDX_CHARACTERS])
    {
        for (auto& tile : layers[LAYER_IDX_LEVEL])
        {
            if (!character->dynamic)
                continue;

            SDL_FRect rectA = {character->position.x + character->collider.x,
                               character->position.y + character->collider.y, character->collider.w,
                               character->collider.h};
            SDL_FRect rectB = {tile->position.x + tile->collider.x,
                               tile->position.y + tile->collider.y, tile->collider.w,
                               tile->collider.h};
            SDL_FRect intersection;

            if (SDL_GetRectIntersectionFloat(&rectA, &rectB, &intersection))
            {
                ResolveCollision(*character, *tile, deltaTime, intersection);
            }
        }
    }
}

void Level::ResolveCollision(GameObject& a, GameObject& b, float deltaTime, SDL_FRect intersection)
{
    if (a.tag == GameObject::Tag::player && b.tag == GameObject::Tag::level)
    {
        // Horizontal Collision
        if (intersection.w < intersection.h)
        {
            if (a.velocity.x > 0)
                a.position.x -= intersection.w;
            else if (a.velocity.x < 0)
                a.position.x += intersection.w;
            a.velocity.x = 0;
        }
        // Vertical Collision
        else
        {
            if (a.velocity.y > 0)
            {
                a.position.y -= intersection.h;
                a.velocity.y = 0;
                Player* p = static_cast<Player*>(&a);
                p->setGrounded(true);
            }
            else if (a.velocity.y < 0)
            {
                a.position.y += intersection.h;
                a.velocity.y = 0;
            }
        }
    }
}
