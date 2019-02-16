#include "motion_sin.h"

#include "../shared.h"

#include <math.h>

#define PI 3.14159265f

MotionSin::MotionSin(float _amplitude, float _period) : Motion(0, _amplitude, _period) {
    period = _period;
}

void MotionSin::advance(float delta) {
    time += delta;
    if (time >= period)
        time -= period;
}

float MotionSin::get_proportion() {
    float linear = Motion::get_proportion();
    return sin(linear * 2 * PI);
}
