#include "animation_looping.h"

#include <string>

#include "../shared.h"

using std::string;

AnimationLooping::AnimationLooping(string base, int frames, float duration) : Animation(base, frames, duration){
    this->loops = -1;
}

AnimationLooping::AnimationLooping(float x, float y, string base, int frames, float duration) : Animation(x, y, base, frames, duration){
    this->loops = -1;
}

AnimationLooping::AnimationLooping(string base, int frames, float duration, int loops) : Animation(base, frames, duration){
    this->loops = loops;
}

AnimationLooping::AnimationLooping(float x, float y, string base, int frames, float duration, int loops) : Animation(x, y, base, frames, duration){
    this->loops = loops;
}

void AnimationLooping::advance(float delta) {
    Animation::advance(delta);

    this->complete = false;

    if (this->loops == 0) {
        this->complete = true;
        return;
    }

    while (this->time >= this->duration) {
        this->time -= this->duration;
        if (this->loops > 0)
            this->loops--;
    }
}

