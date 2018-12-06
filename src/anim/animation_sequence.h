#ifndef GUARD_ANIMATION_SEQUENCE_H
#define GUARD_ANIMATION_SEQUENCE_H

#include "animation.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class AnimationSequence : public Animation {
    public:
        AnimationSequence();
        virtual ~AnimationSequence();
        virtual void advance(float delta) override;
        virtual string sprite() override;
        void add_animation(Animation *anim);
    protected:
        Animation *current_anim;
        vector<Animation*> anims;
};

#endif
