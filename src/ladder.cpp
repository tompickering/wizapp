#include "ladder.h"

#include <string>

#include "shared.h"

Ladder::Ladder(int _x, int _y) : Entity(_x, _y) {
    name = "Ladder";
    flying = true;
    navigable_h = true;
    navigable_v = true;
    base_ladder = false;
    r = 0x56;
    g = 0x08;
    b = 0x59;
}

string Ladder::sprite() {
    if (base_ladder) {
        /* Should be used if this is a 'hanging' ladder; i.e. no ladder beneath this */
        return level_ref->theme_base + "ladder2.png";
    } else {
        return level_ref->theme_base + "ladder1.png";
    }
}
