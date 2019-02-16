#include "static_block.h"

#include "shared.h"

StaticBlock::StaticBlock(int _x, int _y) : Entity(_x, _y) {
    name = "Static Block";
    flying = true;
}

string StaticBlock::sprite() {
    return level_ref->theme_base + "static.png";
}
