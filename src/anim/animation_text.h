#ifndef GUARD_ANIMATION_TEXT_H
#define GUARD_ANIMATION_TEXT_H

#include "animation.h"

#include <string>

using std::string;

class AnimationText : public Animation {
    public:
        AnimationText(float x, float y, string base, int frames, float duration);
        virtual void advance(float delta) override;
        string text;
        unsigned char r;
        unsigned char g;
        unsigned char b;
    protected:
};

#endif
