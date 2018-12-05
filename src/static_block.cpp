#include "static_block.h"

#include "shared.h"

StaticBlock::StaticBlock(int x, int y) : Entity(x, y) {
    this->name = "Static Block";
    this->flying = true;
}

string StaticBlock::sprite() {
    return level_ref->theme_base + "static.png";
}
