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
        Animation(float x, float y, string base, int frames, float duration, bool reverse);
        virtual ~Animation();
        virtual void advance(float delta);
        virtual string sprite();
        /* Rather than relying on state, sample an animation this far through (0-1) */
        virtual string sprite(float);
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
        float duration;

        bool clickable;
    protected:
        string base;
        int frames;
        float frame_duration;
        float time;
        bool reverse;
    private:
        string get_sprite_string(float);
};

#endif
