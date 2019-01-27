#ifndef GUARD_ANIMATION_H
#define GUARD_ANIMATION_H

#include <string>

#include "motion.h"

using std::string;

class Animation {
    public:
        Animation();
        Animation(string base, int frames, float duration);
        Animation(string base, int frames, float duration, bool reverse);
        Animation(float x, float y, string base, int frames, float duration);
        virtual ~Animation();
        virtual void advance(float delta);
        virtual string sprite();
        void reset();
        bool complete;
        bool hide_on_complete;

        /* Positions are only used in (e.g.) intro */
        virtual float get_x();
        virtual float get_y();

        Motion *motion;
        void set_motion(float x_off, float y_off, float duration);

        float x;
        float y;

        bool clickable;
    protected:
        string base;
        int frames;
        float duration;
        float frame_duration;
        float time;
        bool reverse;
};

#endif
