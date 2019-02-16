#include "motion_pulse.h"

#include "../shared.h"

MotionPulse::MotionPulse(float _max, float _duration) : Motion(0, 0, _duration) {
    max = _max;
}

float MotionPulse::get_proportion() {
    float linear = Motion::get_proportion();
    float prop = linear > .5f ? 1.f - linear : linear;
    return 2 * prop;
}
