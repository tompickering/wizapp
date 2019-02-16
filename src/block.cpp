#include "block.h"

#include <string>

#include "shared.h"

using std::string;

Block::Block(int _x, int _y) : Entity(_x, _y) {
    name = "Block";
    r = g = b = 0xAA;
}

bool Block::push(bool left) {
    int target_x = left ? block_x - 1 : block_x + 1;
    if (level_ref->is_empty(target_x, block_y)) {
        move(target_x, block_y);
        return true;
    }
    return false;
}

string Block::sprite() {
    return level_ref->theme_base + "block.png";
}
