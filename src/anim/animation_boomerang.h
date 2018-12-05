#ifndef GUARD_ANIMATION_BOOMERANG_H
#define GUARD_ANIMATION_BOOMERANG_H

#include "animation.h"

#include <string>

using std::string;

class AnimationBoomerang : public Animation {
    public:
        AnimationBoomerang(string base, int frames, float duration);
        AnimationBoomerang(float x, float y, string base, int frames, float duration);
        virtual void advance(float delta) override;
        void set_iterations(int iterations);
    protected:
        int iterations;
};

#endif
