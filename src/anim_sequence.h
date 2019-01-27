#ifndef GUARD_ANIM_SEQUENCE_H
#define GUARD_ANIM_SEQUENCE_H

#include <vector>

#include "scene.h"
#include "anim/animation.h"

using std::vector;

class AnimSequence {
    public:
        AnimSequence();
        virtual ~AnimSequence();
        virtual void update(float delta_time);
        bool complete;
    protected:
        virtual void next_event();
        float time;
        float next_event_countdown;
        vector<Animation*> current_anims;
        Scene *current_scene;
        virtual void clear_anims();
};

#endif
