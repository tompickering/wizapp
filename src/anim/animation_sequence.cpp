#include "animation_sequence.h"

#include <string>
#include <vector>

#include "../shared.h"

using std::string;
using std::vector;

AnimationSequence::AnimationSequence() : Animation() {
    this->current_anim = NULL;
}

AnimationSequence::~AnimationSequence() {
    for (unsigned int i = 0; i < this->anims.size(); ++i) {
        delete this->anims.at(i);
    }
}

void AnimationSequence::add_animation(Animation *anim) {
    this->anims.push_back(anim);
}

void AnimationSequence::advance(float delta) {
    bool all_complete = true;
    for (unsigned int i = 0; i < this->anims.size(); ++i) {
        if (!this->anims.at(i)->complete) {
            if (this->current_anim && this->current_anim != this->anims.at(i)) {
                this->anims.at(i)->x = this->current_anim->get_x();
                this->anims.at(i)->y = this->current_anim->get_y();
            }
            this->current_anim = this->anims.at(i);
            all_complete = false;
            break;
        }
    }

    if (this->current_anim) {
        this->current_anim->advance(delta);
        this->x = this->current_anim->get_x();
        this->y = this->current_anim->get_y();
    }

    this->complete = all_complete;
}

string AnimationSequence::sprite() {
    if (!this->current_anim)
        return "";
    return this->current_anim->sprite();
}

