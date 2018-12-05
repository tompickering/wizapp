#ifndef GUARD_ANIMATION_LOOPING_H
#define GUARD_ANIMATION_LOOPING_H

#include "animation.h"

#include <string>

using std::string;

class AnimationLooping : public Animation {
    public:
        AnimationLooping(string base, int frames, float duration);
        AnimationLooping(float x, float y, string base, int frames, float duration);
        virtual void advance(float delta) override;
};

#endif
