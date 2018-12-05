#include "motion.h"

#include "../shared.h"

Motion::Motion(float x_target_offset, float y_target_offset, float duration) {
    this->x_target_offset = x_target_offset;
    this->y_target_offset = y_target_offset;
    this->duration = duration;
    this->reset();
}

Motion::~Motion() {
}

void Motion::reset() {
    this->time = 0;
}

void Motion::advance(float delta) {
    this->time += delta;
    if (this->time > this->duration)
        this->time = this->duration;
}

float Motion::get_x_offset() {
    return x_target_offset * this->time / this->duration;
}

float Motion::get_y_offset() {
    return y_target_offset * this->time / this->duration;
}
