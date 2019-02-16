#ifndef GUARD_ANIMATION_TEXT_H
#define GUARD_ANIMATION_TEXT_H

#include "animation.h"

#include <string>

using std::string;

class AnimationText : public Animation {
    public:
        AnimationText(void* font, float x, float y, string base, int frames, float duration);
        virtual void advance(float delta) override;
        string text;
        void *font;
        float get_brightness();
    protected:
        float brightness;
};

#endif
