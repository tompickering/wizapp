#include "animation_sequence.h"

#include <string>
#include <vector>

#include "../shared.h"

using std::string;
using std::vector;

AnimationSequence::AnimationSequence() : Animation() {
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
            this->anims.at(i)->advance(delta);
            this->x = this->anims.at(i)->get_x();
            this->y = this->anims.at(i)->get_y();
            all_complete = false;
            break;
        }
    }

    this->complete = all_complete;
}

string AnimationSequence::sprite() {
    Animation *current_anim;
    for (unsigned int i = 0; i < this->anims.size(); ++i) {
        current_anim = this->anims.at(i);
        if (!current_anim->complete)
            break;
    }

    return current_anim->sprite();
}

