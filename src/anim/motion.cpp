#include "motion.h"

#include "../shared.h"

Motion::Motion(float _x_target_offset, float _y_target_offset, float _duration) {
    x_target_offset = _x_target_offset;
    y_target_offset = _y_target_offset;
    duration = _duration;
    reset();
}

Motion::~Motion() {
}

void Motion::reset() {
    time = 0;
}

void Motion::advance(float delta) {
    time += delta;
    if (time > duration)
        time = duration;
}

float Motion::get_x_offset() {
    return x_target_offset * time / duration;
}

float Motion::get_y_offset() {
    return y_target_offset * time / duration;
}
