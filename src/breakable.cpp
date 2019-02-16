#include "breakable.h"

#include <string>

#include "level.h"
#include "anim/animation.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Breakable::Breakable(int _x, int _y) : Entity(_x, _y) {
    name = "Breakable";
    flying = true;
    g = 0xFF;

    anim_idle = new AnimationBoomerang(
        string(level_ref->theme_base + "breakable/"), 5, 0.2f);
    anim_break = new Animation(
        string(level_ref->theme_base + "break/"), 10, 0.1f);

    anim_break->hide_on_complete = true;

    reset();
}

Breakable::~Breakable() {
    delete anim_idle;
    delete anim_break;
}

void Breakable::update(float delta_time) {
    Entity::update(delta_time);
    if (breaking) {
        anim_break->advance(delta_time);
    } else {
        anim_idle->advance(delta_time);
    }

    if (anim_break->complete) {
        navigable_h = true;
    }
}

void Breakable::reset() {
    Entity::reset();
    navigable_h = false;
    breaking = false;
    anim_idle->reset();
    anim_break->reset();
}

string Breakable::sprite() {
    if (breaking) {
        return anim_break->sprite();
    }
    return anim_idle->sprite();
}

bool Breakable::break_block() {
    if (!breaking) {
        breaking = true;
        if (level_ref->theme == Jazz) {
            audio_manager.play_sfx(FrequencyClear);
        } else {
            audio_manager.play_sfx(Break);
        }
        return true;
    }
    return false;
}
