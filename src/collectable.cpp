#include "collectable.h"

#include <string>

#include "anim/animation.h"
#include "anim/animation_looping.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Collectable::Collectable(int x, int y, bool flying) : Entity(x, y) {
    name = "Collectable";
    flying = flying;
    navigable_h = true;
    collected = false;
    string type = flying ? "2" : "1";

    /* Defaults */
    ctype = flying ? FMushroom : Flower;
    float duration = 0.35f;

    if (flying) {
        switch (level_ref->theme) {
            case Valley:
                ctype = FMushroom;
                break;
            case Industrial:
                ctype = FMushroom;
                break;
            case Sun:
                ctype = Energy;
                break;
            case Snowman:
                ctype = FShadow;
                break;
            case Jazz:
                ctype = Note2;
                break;
            case Stereo:
                ctype = Note3;
                break;
            case Star:
                ctype = FStar1;
                break;
            case Alien:
                ctype = FStar2;
                break;
            case Fantasia1:
                ctype = FDiamond;
                break;
            case Fantasia2:
                ctype = FUnknown;
                break;
        }
    } else {
        switch (level_ref->theme) {
            case Valley:
                ctype = Flower;
                duration = 0.2f;
                break;
            case Industrial:
                ctype = Mushroom;
                break;
            case Sun:
                ctype = Atom;
                break;
            case Snowman:
                ctype = Shadow;
                break;
            case Jazz:
                ctype = Note1;
                break;
            case Stereo:
                ctype = Clef;
                duration = 0.5f;
                break;
            case Star:
                ctype = Star1;
                break;
            case Alien:
                ctype = Star2;
                break;
            case Fantasia1:
                ctype = Diamond;
                break;
            case Fantasia2:
                ctype = Unknown;
                break;
        }
    }

    switch(ctype) {
        case Flower:
        case Note3:
            anim_idle = new AnimationBoomerang(
                    string(level_ref->theme_base + "collectable" + type + "/"), 10, duration);
            break;
        default:
            anim_idle = new AnimationLooping(
                    string(level_ref->theme_base + "collectable" + type + "/"), 10, duration);
            break;
    }

    anim_collect = new Animation(level_ref->theme_base + "collect/", 10, 0.2f);
    anim_collect->hide_on_complete = true;

    if (flying) {
        r = 0xEE;
        g = 0x42;
        b = 0xF4;
    } else {
        r = g = 0xFF;
    }
}

Collectable::~Collectable() {
    delete anim_idle;
    delete anim_collect;
}

void Collectable::update(float delta_time) {
    Entity::update(delta_time);
    if (collected) {
        anim_collect->advance(delta_time);
    } else {
        anim_idle->advance(delta_time);
    }
}

void Collectable::reset() {
    Entity::reset();
    collected = false;
    navigable_v = false;
    anim_idle->reset();
    anim_collect->reset();
}

void Collectable::collect() {
    if (!collected) {
        logger.info("Collected!");
        audio_manager.play_sfx(Collect);
        collected = true;
        navigable_v = true;
    }
}

string Collectable::sprite() {
    if (collected) {
        return anim_collect->sprite();
    }
    return anim_idle->sprite();
}
