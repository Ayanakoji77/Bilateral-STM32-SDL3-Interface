#pragma once
#include <vector>

#include "core/animation.h"
#include "gameobject.h"

enum class PlayerState
{
    Idle,
    Running,
    Jumping,
};

class Player : public GameObject
{
   private:
    PlayerState state = PlayerState::Idle;

    // constants for the player
    float maxSpeedX = 80.0f;
    float direction = 1.0f;
    glm::vec2 acceleration{250.0f, 0.0f};
    float jump_power = -20.0f;
    std::vector<Animation> animations;
    int currentAnim = 0;
    bool grounded = false;

   public:
    Player(SDL_Texture* atlasTexture);
    void update(float deltaTime, const bool* keys) override;
    void Render(SDL_Renderer* renderer) override;
    void setGrounded(bool val) { grounded = val; }
    bool isGrounded() { return grounded; }
};
