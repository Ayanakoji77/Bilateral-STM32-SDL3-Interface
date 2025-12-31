#pragma once
#include <SDL3/SDL_rect.h>

#include <array>
#include <memory>
#include <vector>

#include "core/resourceManager.h"
#include "gameobject.h"

class Level
{
   private:
    // layers background , player and blocks
    static const int LAYER_IDX_LEVEL = 0;
    static const int LAYER_IDX_CHARACTERS = 1;
    static const int TOTAL_LAYERS = 2;

    std::array<std::vector<std::unique_ptr<GameObject>>, TOTAL_LAYERS> layers;

    static const int MAP_ROWS = 5;
    static const int MAP_COLS = 50;
    static const int TILE_SIZE = 32;

    void CheckCollisions(float deltaTime);
    void ResolveCollision(GameObject& a, GameObject& b, float deltaTime, SDL_FRect intersection);

   public:
    void LoadMap(ResourceManager* res);
    void Update(float deltaTime, const bool* keys);
    void Render(SDL_Renderer* renderer);
};
