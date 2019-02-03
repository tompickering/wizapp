#ifndef GUARD_OUTRO_H
#define GUARD_OUTRO_H

#include "anim_sequence.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"

#define PORTAL_Y .665f
#define PORTAL_CLOSED_TIME 1.f
#define PORTAL_OPEN_TIME .5f
#define PORTAL_SHUT_TIME .5f
#define SLIDE_TIME 3.f
#define MUSIC_FADE_TIME 2.f
#define BYE_TIME 10.f

using std::vector;

class Outro : public AnimSequence {
    public:
        Outro();
    protected:
        virtual void next_event() override;
    private:
        float world_times[6] = {1.f, 2.f, 1.f, 2.f, 1.f, 2.f};

        void start();
        void portal_open();
        void world(int);
        void world_end(int);
        void end();

        bool world_end_required;
        bool start_done;
        int on_world;
        bool fantasia_done;
        bool end_done;
};

#endif
