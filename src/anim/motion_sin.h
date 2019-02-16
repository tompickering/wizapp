#ifndef GUARD_MOTION_SIN_H
#define GUARD_MOTION_SIN_H

#include "motion.h"

class MotionSin : public Motion {
    public:
        MotionSin(float, float);
        virtual void advance(float) override;
        float period;
    protected:
        virtual float get_proportion() override;
};

#endif
