#include "animation_looping.h"

#include <string>

#include "../shared.h"

using std::string;

AnimationLooping::AnimationLooping(string base, int frames, float duration) : Animation(base, frames, duration){
}

AnimationLooping::AnimationLooping(float x, float y, string base, int frames, float duration) : Animation(x, y, base, frames, duration){
}

void AnimationLooping::advance(float delta) {
    this->time += delta;
    while (this->time >= this->duration)
        this->time -= this->duration;
}

