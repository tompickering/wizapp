#ifndef GUARD_EVENT_H
#define GUARD_EVENT_H

#include "anim_sequence.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"

#define START_TIME 0.f


using std::vector;

class Event : public AnimSequence {
    public:
        Event();
    protected:
        virtual void next_event() override;
    private:
        float skip_duration;
        bool skip_pressed;
        float skip_time;
        void winds();
        void start();

        bool start_done;
};

#endif
