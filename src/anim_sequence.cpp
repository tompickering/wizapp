#include "anim_sequence.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"
#include "anim/animation_boomerang.h"
#include "anim/animation_looping.h"

#include "shared.h"

using std::vector;

AnimSequence::AnimSequence() {
    this->time = 0.f;
    this->complete = false;
    this->next_event_countdown = 0.f;
    this->current_scene = NULL;
}

AnimSequence::~AnimSequence() {
    this->clear_anims();
}

void AnimSequence::next_event() {
}

void AnimSequence::update(float delta_time) {
    this->time += delta_time;

    if ((this->next_event_countdown <= 0.f && !this->current_scene)
        || (this->current_scene && this->current_scene->complete)) {
        this->next_event();
    }

    if (this->current_scene) {
        this->current_scene->update(delta_time);
        draw_manager.update(this->current_scene);
    } else {
        for (unsigned int i = 0; i < this->current_anims.size(); ++i) {
            this->current_anims.at(i)->advance(delta_time);
        }
        draw_manager.update(this->current_anims);

        this->next_event_countdown -= delta_time;
    }
}

void AnimSequence::clear_anims() {
    for (unsigned int i = 0; i < this->current_anims.size(); ++i) {
        delete this->current_anims.at(i);
    }
    this->current_anims.clear();

    if (this->current_scene) {
        delete this->current_scene;
        this->current_scene = NULL;
    }
}

