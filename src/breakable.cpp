#include "breakable.h"

#include <string>

#include "anim/animation.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Breakable::Breakable(int x, int y) : Entity(x, y) {
    this->name = "Breakable";
    this->flying = true;
    this->g = 0xFF;

    this->anim_idle = new AnimationBoomerang(
            string(level_ref->theme_base + "breakable/"), 5, 0.2f);
    this->anim_break = new Animation(
            string(level_ref->theme_base + "break/"), 10, 0.1f);

    this->anim_break->hide_on_complete = true;

    this->reset();
}

Breakable::~Breakable() {
    delete this->anim_idle;
    delete this->anim_break;
}

void Breakable::update(float delta_time) {
    Entity::update(delta_time);
    if (this->breaking) {
        this->anim_break->advance(delta_time);
    } else {
        this->anim_idle->advance(delta_time);
    }

    if (this->anim_break->complete) {
        this->navigable_h = true;
    }
}

void Breakable::reset() {
    Entity::reset();
    this->navigable_h = false;
    this->breaking = false;
    this->anim_idle->reset();
    this->anim_break->reset();
}

string Breakable::sprite() {
    if (this->breaking) {
        return this->anim_break->sprite();
    }
    return this->anim_idle->sprite();
}

bool Breakable::break_block() {
    if (!this->breaking) {
        this->breaking = true;
        return true;
    }
    return false;
}
