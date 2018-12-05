#include "collectable.h"

#include <string>

#include "anim/animation.h"
#include "anim/animation_looping.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Collectable::Collectable(int x, int y, bool flying) : Entity(x, y) {
    this->name = "Collectable";
    this->flying = flying;
    this->navigable_h = true;
    this->collected = false;
    string type = flying ? "2" : "1";

    /* Defaults */
    this->ctype = flying ? FMushroom : Flower;
    float duration = 0.35f;

    if (flying) {
        switch (level_ref->theme) {
            case Valley:
                this->ctype = FMushroom;
                break;
            case Industrial:
                this->ctype = FMushroom;
                break;
            case Sun:
                this->ctype = Energy;
                break;
            case Snowman:
                this->ctype = FShadow;
                break;
            case Jazz:
                this->ctype = Note2;
                break;
            case Stereo:
                this->ctype = Note3;
                break;
            case Star:
                this->ctype = FStar1;
                break;
            case Alien:
                this->ctype = FStar2;
                break;
            case Fantasia1:
                this->ctype = FDiamond;
                break;
            case Fantasia2:
                this->ctype = FUnknown;
                break;
        }
    } else {
        switch (level_ref->theme) {
            case Valley:
                this->ctype = Flower;
                duration = 0.2f;
                break;
            case Industrial:
                this->ctype = Mushroom;
                break;
            case Sun:
                this->ctype = Atom;
                break;
            case Snowman:
                this->ctype = Shadow;
                break;
            case Jazz:
                this->ctype = Note1;
                break;
            case Stereo:
                this->ctype = Clef;
                duration = 0.5f;
                break;
            case Star:
                this->ctype = Star1;
                break;
            case Alien:
                this->ctype = Star2;
                break;
            case Fantasia1:
                this->ctype = Diamond;
                break;
            case Fantasia2:
                this->ctype = Unknown;
                break;
        }
    }

    switch(this->ctype) {
        case Flower:
        case Note3:
            this->anim_idle = new AnimationBoomerang(
                    string(level_ref->theme_base + "collectable" + type + "/"), 10, duration);
            break;
        default:
            this->anim_idle = new AnimationLooping(
                    string(level_ref->theme_base + "collectable" + type + "/"), 10, duration);
            break;
    }

    this->anim_collect = new Animation(level_ref->theme_base + "collect/", 10, 0.15f);
    if (this->flying) {
        this->r = 0xEE;
        this->g = 0x42;
        this->b = 0xF4;
    } else {
        this->r = this->g = 0xFF;
    }
}

Collectable::~Collectable() {
    delete this->anim_idle;
    delete this->anim_collect;
}

void Collectable::update(float delta_time) {
    Entity::update(delta_time);
    if (this->collected) {
        this->anim_collect->advance(delta_time);
    } else {
        this->anim_idle->advance(delta_time);
    }
}

void Collectable::reset() {
    Entity::reset();
    this->collected = false;
    this->navigable_v = false;
    this->anim_idle->reset();
    this->anim_collect->reset();
}

void Collectable::collect() {
    if (!this->collected) {
        logger.info("Collected!");
        audio_manager.play_sfx(Collect);
        this->collected = true;
        this->navigable_v = true;
    }
}

string Collectable::sprite() {
    if (this->collected) {
        return this->anim_collect->sprite();
    }
    return this->anim_idle->sprite();
}
