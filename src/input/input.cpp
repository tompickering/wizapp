#include "input.h"

InputManager::InputManager() {
    click_pos.x = -1;
    click_pos.y = -1;
}

bool InputManager::read(Input input, bool reset) {
    bool result;
    switch (input) {
        case Up:
            result = this->up;
            this->up = this->up && !reset;
            break;
        case Down:
            result = this->down;
            this->down = this->down && !reset;
            break;
        case Left:
            result = this->left;
            this->left = this->left && !reset;
            break;
        case Right:
            result = this->right;
            this->right = this->right && !reset;
            break;
        case Space:
            result = this->space;
            this->space = this->space && !reset;
            break;
        case R:
            result = this->r;
            this->r = this->r && !reset;
            break;
        case LShift:
            result = this->lshift;
            this->lshift = this->lshift && !reset;
            break;
    }
    return result;
}

ClickPos InputManager::read_click() {
    ClickPos result = click_pos;
    click_pos.x = -1;
    click_pos.y = -1;
    return result;
}
