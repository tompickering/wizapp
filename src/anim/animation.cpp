#include "animation.h"

#include <string>
#include <cstdio>

#include "../shared.h"

using std::string;

Animation::Animation() {
    this->base = "";
    this->frames = 1;
    this->duration = 1.f;
    this->frame_duration = 1.f;
    this->motion = NULL;
    this->reset();
    this->x = 0.f;
    this->y = 0.f;
    this->reverse = false;
    this->hide_on_complete = false;
    this->clickable = true;
}

Animation::Animation(string base, int frames, float duration) {
    this->base = base;
    this->frames = frames;
    this->duration = duration;
    this->frame_duration = duration / (float) frames;
    this->motion = NULL;
    this->reset();
    this->x = 0.f;
    this->y = 0.f;
    this->reverse = false;
    this->hide_on_complete = false;
    this->clickable = true;
}

Animation::Animation(string base, int frames, float duration, bool reverse) {
    this->base = base;
    this->frames = frames;
    this->duration = duration;
    this->frame_duration = duration / (float) frames;
    this->motion = NULL;
    this->reset();
    this->x = 0.f;
    this->y = 0.f;
    this->reverse = reverse;
    this->hide_on_complete = false;
    this->clickable = true;

    if (this->reverse) {
        this->time = duration;
    }
}

Animation::Animation(float x, float y, string base, int frames, float duration) {
    this->base = base;
    this->frames = frames;
    this->duration = duration;
    this->frame_duration = duration / (float) frames;
    this->motion = NULL;
    this->reset();
    this->x = x;
    this->y = y;
    this->reverse = false;
    this->hide_on_complete = false;
    this->clickable = true;
}

Animation::~Animation() {
    if (this->motion)
        delete this->motion;
}

void Animation::advance(float delta) {
    if (this->reverse) {
        this->time -= delta;
    } else {
        this->time += delta;
    }

    if (this->motion) {
        this->motion->advance(delta);
    }

    if (this->time < 0.f) {
        this->time = 0.f;
        if (this->reverse)
            this->complete = true;
    }

    if (this->time > this->duration) {
        this->time = this->duration;
        if (!this->reverse)
            this->complete = true;
    }
}

void Animation::reset() {
    this->time = 0.f;
    this->complete = false;
    if (this->motion)
        this->motion->reset();
}

string Animation::sprite() {
    if (this->complete && this->hide_on_complete)
        return "";

    if (this->frames > 1) {
        int frame = (int)((float) this->frames * this->time / this->duration) + 1;
        if (frame > this->frames)
            frame = this->frames;
        char frame_str[3];
        frame_str[2] = '\0';
        sprintf(frame_str, "%02d", frame);
        return this->base + frame_str + ".png";
    }
    return this->base + ".png";
}

void Animation::set_motion(float x_off, float y_off, float duration) {
    if (this->motion)
        delete this->motion;
    this->motion = new Motion(x_off, y_off, duration);
}

float Animation::get_x() {
    if (this->motion)
        return this->x + this->motion->get_x_offset();
    return this->x;
}

float Animation::get_y() {
    if (this->motion)
        return this->y + this->motion->get_y_offset();
    return this->y;
}
