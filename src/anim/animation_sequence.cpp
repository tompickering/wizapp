#include "animation_sequence.h"

#include <string>
#include <vector>

#include "../shared.h"

using std::string;
using std::vector;

AnimationSequence::AnimationSequence() : Animation() {
    current_anim = NULL;
    duration = 0.f;
}

AnimationSequence::~AnimationSequence() {
    for (unsigned int i = 0; i < anims.size(); ++i) {
        delete anims.at(i);
    }
}

void AnimationSequence::add_animation(Animation *anim) {
    anims.push_back(anim);
    duration += anim->duration;
}

void AnimationSequence::advance(float delta) {
    bool all_complete = true;
    for (unsigned int i = 0; i < anims.size(); ++i) {
        if (!anims.at(i)->complete) {
            if (current_anim && current_anim != anims.at(i)) {
                anims.at(i)->x = current_anim->get_x();
                anims.at(i)->y = current_anim->get_y();
            }
            current_anim = anims.at(i);
            all_complete = false;
            break;
        }
    }

    if (current_anim) {
        current_anim->advance(delta);
        x = current_anim->get_x();
        y = current_anim->get_y();
    }

    complete = all_complete;
}

string AnimationSequence::sprite() {
    if (!current_anim)
        return "";
    return current_anim->sprite();
}

string AnimationSequence::sprite(float interp) {
    float tracked_duration = 0.f;
    for (unsigned int i = 0; i < anims.size(); ++i) {
        float a_duration = anims.at(i)->duration;
        float base_duration = tracked_duration;
        tracked_duration += a_duration;
        if (tracked_duration / duration >= interp) {
            /* Interpolation point somewhere inside this anim */
            float start = base_duration / duration;
            float end = tracked_duration / duration;
            float a_point = (interp - start) / (end - start);
            return anims.at(i)->sprite(a_point);
        }
    }
    return "";
}

