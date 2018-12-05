#include "animation_boomerang.h"

#include <string>
#include "../shared.h"

using std::string;

AnimationBoomerang::AnimationBoomerang(string base, int frames, float duration) : Animation(base, frames, duration){
    this->iterations = -1;
}

AnimationBoomerang::AnimationBoomerang(float x, float y, string base, int frames, float duration) : Animation(x, y, base, frames, duration){
    this->iterations = -1;
}

void AnimationBoomerang::set_iterations(int iterations) {
    this->iterations = iterations;
}

void AnimationBoomerang::advance(float delta) {
    Animation::advance(delta);
    if (this->time >= (this->duration - this->frame_duration / 2.f)) {
        reverse = true;
    } else if (this->time < (this->frame_duration / 2.f)) {
        if (reverse && this->iterations > 0)
            this->iterations--;
        reverse = false;
    }

    if (this->iterations == 0)
        this->complete = true;
}

