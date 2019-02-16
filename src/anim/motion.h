#ifndef GUARD_MOTION_H
#define GUARD_MOTION_H

class Motion {
    public:
        Motion(float x_target_offset, float y_target_offset, float duration);
        virtual ~Motion();
        void reset();
        virtual void advance(float delta);
        float x_target_offset;
        float y_target_offset;
        float brightness_target_offset;
        float get_x_offset();
        float get_y_offset();
        float get_brightness_offset();
        float start_delay;
    protected:
        float time;
        float duration;
        virtual float get_proportion();
};

#endif
