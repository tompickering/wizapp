#include "collectable.h"

#include <string>

#include "anim/animation.h"
#include "anim/animation_looping.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Collectable::Collectable(int _x, int _y, bool _flying) : Entity(_x, _y) {
    name = "Collectable";
    flying = _flying;
    navigable_h = true;
    collected = false;
    shield = 0.f;

    /* Default */
    ctype = flying ? FMushroom : Flower;

    if (flying) {
        switch (level_ref->theme) {
            case Valley:
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

    anim_idle = make_anim(ctype);

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

Animation* Collectable::make_anim(CollectableType type) {
    float duration = 0.35f;

    string theme;
    switch (type) {
        case Flower:
        case FMushroom:
            theme = "01";
            break;
        case Mushroom:
            theme = "02";
            break;
        case Atom:
        case Energy:
            theme = "03";
            break;
        case Shadow:
        case FShadow:
            theme = "04";
            break;
        case Note1:
        case Note2:
            theme = "05";
            break;
        case Clef:
        case Note3:
            theme = "06";
            break;
        case Star1:
        case FStar1:
            theme = "07";
            break;
        case Star2:
        case FStar2:
            theme = "08";
            break;
        case Diamond:
        case FDiamond:
            theme = "09";
            break;
        case Unknown:
        case FUnknown:
            theme = "10";
            break;
    }
    string base = "assets/img/themes/" + theme + "/";

    string flying_str = "1";
    switch (type) {
        case FMushroom:
        case Energy:
        case FShadow:
        case Note2:
        case Note3:
        case FStar1:
        case FStar2:
        case FDiamond:
        case FUnknown:
            flying_str = "2";
            break;
        default:
            break;
    }

    switch (type) {
        case Flower:
            duration = 0.2f;
        case Clef:
            duration = 0.5f;
        default:
            break;
    }

    switch (type) {
        case Flower:
        case Note3:
            return new AnimationBoomerang(
                           string(base + "collectable" + flying_str + "/"), 10, duration);
        default:
            break;
    }

    return new AnimationLooping(
                   string(base + "collectable" + flying_str + "/"), 10, duration);

}

float Collectable::get_shield() {
    return shield;
}

void Collectable::update(float delta_time) {
    Entity::update(delta_time);
    if (collected) {
        anim_collect->advance(delta_time);
    } else {
        anim_idle->advance(delta_time);
    }

    /* Check if we're being stood on - shield animation */
    if ((level_ref->wizard->real_y == real_y - 1
             && level_ref->wizard->real_x >= real_x - 0.5
             && level_ref->wizard->real_x <= real_x + 0.5)
        || (level_ref->blob &&
            (level_ref->blob->real_y == real_y - 1
             && level_ref->blob->real_x == real_x))) {
        shield += delta_time * 12.f;
    } else {
        shield -= delta_time * 12.f;
    }

    if (shield > 1.f)
        shield = 1.f;
    else if (shield < 0)
        shield = 0.f;
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
