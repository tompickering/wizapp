#include "motion.h"

#include "../shared.h"

Motion::Motion(float _x_target_offset, float _y_target_offset, float _duration) {
    x_target_offset = _x_target_offset;
    y_target_offset = _y_target_offset;
    brightness_target_offset = 0.f;
    start_delay = 0.f;
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
    if (time > duration + start_delay)
        time = duration + start_delay;
}

float Motion::get_proportion() {
    float t = clamp(time - start_delay, 0.f, duration);
    return t / duration;
}

float Motion::get_x_offset() {
    return x_target_offset * get_proportion();
}

float Motion::get_y_offset() {
    return y_target_offset * get_proportion();
}

float Motion::get_brightness_offset() {
    return brightness_target_offset * get_proportion();
}
