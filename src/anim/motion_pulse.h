#ifndef GUARD_MOTION_PULSE_H
#define GUARD_MOTION_PULSE_H

#include "motion.h"

class MotionPulse : public Motion {
    public:
        MotionPulse(float, float);
        float max;
    protected:
        virtual float get_proportion() override;
};

#endif
