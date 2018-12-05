#include "block.h"

#include <string>

#include "shared.h"

using std::string;

Block::Block(int x, int y) : Entity(x, y) {
    this->name = "Block";
    this->r = this->g = this->b = 0xAA;
}

bool Block::push(bool left) {
    int target_x = left ? this->block_x - 1 : this->block_x + 1;
    if (level_ref->is_empty(target_x, this->block_y)) {
        this->move(target_x, this->block_y);
        return true;
    }
    return false;
}

string Block::sprite() {
    return level_ref->theme_base + "block.png";
}
