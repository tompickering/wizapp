#ifndef GUARD_ANIMATION_LOOPING_H
#define GUARD_ANIMATION_LOOPING_H

#include "animation.h"

#include <string>

using std::string;

class AnimationLooping : public Animation {
    public:
        AnimationLooping(string base, int frames, float duration);
        AnimationLooping(float x, float y, string base, int frames, float duration);
        AnimationLooping(string base, int frames, float duration, int loops);
        AnimationLooping(float x, float y, string base, int frames, float duration, int loops);
        virtual void advance(float delta) override;
    protected:
        int loops;
};

#endif
