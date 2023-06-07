#include "input.h"

InputManager::InputManager() {
    click_pos.x = -1;
    click_pos.y = -1;
}

bool InputManager::read(Input input, bool reset) {
    bool result = false;
    switch (input) {
        case Up:
        case P:
            result = up;
            up = up && !reset;
            break;
        case Down:
        case L:
            result = down;
            down = down && !reset;
            break;
        case Left:
        case Z:
            result = left;
            left = left && !reset;
            break;
        case Right:
        case X:
            result = right;
            right = right && !reset;
            break;
        case Space:
            result = space;
            space = space && !reset;
            break;
        case R:
            result = r;
            r = r && !reset;
            break;
        case Q:
            result = q;
            q = q && !reset;
            break;
        case LShift:
            result = lshift;
            lshift = lshift && !reset;
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
