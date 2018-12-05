#include "ladder.h"

#include <string>

#include "shared.h"

Ladder::Ladder(int x, int y) : Entity(x, y) {
    this->name = "Ladder";
    this->flying = true;
    this->navigable_h = true;
    this->navigable_v = true;
    this->base_ladder = false;
    this->r = 0x56;
    this->g = 0x08;
    this->b = 0x59;
}

string Ladder::sprite() {
    if (this->base_ladder) {
        /* Should be used if this is a 'hanging' ladder; i.e. no ladder beneath this */
        return level_ref->theme_base + "ladder2.png";
    } else {
        return level_ref->theme_base + "ladder1.png";
    }
}
