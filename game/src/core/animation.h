#pragma once
#include <SDL3/SDL_rect.h>

#include "timer.h"

class Animation
{
   private:
    int frameCount;
    int row_animation;
    int frameWidth;
    int frameHeight;
    Timer timer;

   public:
    Animation() : frameCount(0), row_animation(0), frameWidth(0), frameHeight(0), timer(0) {}
    Animation(int frame_count, float length, int row, int w, int h)
        : frameCount(frame_count), row_animation(row), frameWidth(w), frameHeight(h), timer(length)
    {
    }

    float getLength() const { return timer.getLength(); }

    int currentFrame() const
    {
        // divisio by zero not happen
        if (frameCount == 0)
            return 0;

        // raw frame index cal
        int frame = static_cast<int>(timer.getTime() / timer.getLength() * frameCount);
        // to not get the frame count = frame , length = index out of bounds saved
        if (frame >= frameCount)
        {
            frame = frameCount - 1;
        }
        return frame;
    }
    SDL_FRect GetCurrentFrameSrc() const
    {
        int currentFrameIndex = 0;
        if (frameCount > 0)
        {
            currentFrameIndex = static_cast<int>(timer.getTime() / timer.getLength() * frameCount);
            if (currentFrameIndex >= frameCount)
            {
                currentFrameIndex = frameCount - 1;
            }
        }
        SDL_FRect src;
        src.x = static_cast<float>(currentFrameIndex * frameWidth);
        src.y = static_cast<float>(row_animation * frameHeight);  // Jump down to the correct row
        src.w = static_cast<float>(frameWidth);
        src.h = static_cast<float>(frameHeight);

        return src;
    }
    void step(float deltaTime) { timer.step(deltaTime); }
};
