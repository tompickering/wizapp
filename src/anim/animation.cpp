#include "animation.h"

#include <string>
#include <cstdio>

#include "../shared.h"

using std::string;

Animation::Animation() {
    base = "";
    frames = 1;
    duration = 1.f;
    frame_duration = 1.f;
    motion = NULL;
    reset();
    x = 0.f;
    y = 0.f;
    reverse = false;
    hide_on_complete = false;
    clickable = true;
}

Animation::Animation(string _base, int _frames, float _duration) {
    base = _base;
    frames = _frames;
    duration = _duration;
    frame_duration = duration / (float) frames;
    motion = NULL;
    reset();
    x = 0.f;
    y = 0.f;
    reverse = false;
    hide_on_complete = false;
    clickable = true;
}

Animation::Animation(string _base, int _frames, float _duration, bool _reverse) {
    base = _base;
    frames = _frames;
    duration = _duration;
    frame_duration = duration / (float) frames;
    motion = NULL;
    reset();
    x = 0.f;
    y = 0.f;
    reverse = _reverse;
    hide_on_complete = false;
    clickable = true;

    if (reverse) {
        time = duration;
    }
}

Animation::Animation(float _x, float _y, string _base, int _frames, float _duration) {
    base = _base;
    frames = _frames;
    duration = _duration;
    frame_duration = duration / (float) frames;
    motion = NULL;
    reset();
    x = _x;
    y = _y;
    reverse = false;
    hide_on_complete = false;
    clickable = true;
}

Animation::Animation(float _x, float _y, string _base, int _frames, float _duration, bool _reverse) {
    base = _base;
    frames = _frames;
    duration = _duration;
    frame_duration = duration / (float) frames;
    motion = NULL;
    reset();
    x = _x;
    y = _y;
    reverse = _reverse;
    hide_on_complete = false;
    clickable = true;

    if (reverse) {
        time = duration;
    }
}

Animation::~Animation() {
    if (motion)
        delete motion;
}

void Animation::advance(float delta) {
    if (reverse) {
        time -= delta;
    } else {
        time += delta;
    }

    if (motion) {
        motion->advance(delta);
    }

    if (time < 0.f) {
        time = 0.f;
        if (reverse)
            complete = true;
    }

    if (time > duration) {
        time = duration;
        if (!reverse)
            complete = true;
    }
}

void Animation::reset() {
    time = 0.f;
    complete = false;
    if (motion)
        motion->reset();
}

string Animation::sprite() {
    if (complete && hide_on_complete)
        return "";

    return get_sprite_string(time / duration);
}

string Animation::sprite(float interp) {
    if (complete && hide_on_complete)
        return "";

    return get_sprite_string(interp);
}

string Animation::get_sprite_string(float interp) {
    if (frames > 1) {
        int frame = (int)((float) frames * interp) + 1;
        if (frame > frames)
            frame = frames;
        char frame_str[3];
        frame_str[2] = '\0';
        sprintf(frame_str, "%02d", frame);
        return base + frame_str + ".png";
    }
    return base + ".png";
}


void Animation::set_motion(float x_off, float y_off, float duration) {
    if (motion)
        delete motion;
    motion = new Motion(x_off, y_off, duration);
}

float Animation::get_x() {
    if (motion)
        return x + motion->get_x_offset();
    return x;
}

float Animation::get_y() {
    if (motion)
        return y + motion->get_y_offset();
    return y;
}
