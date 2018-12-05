#ifndef GUARD_INPUTMANAGER_H
#define GUARD_INPUTMANAGER_H

enum Input {
    Up,
    Down,
    Left,
    Right,
    Space,
    R
};

class InputManager {
    public:
        bool read(Input, bool reset);
    protected:
        bool up;
        bool down;
        bool left;
        bool right;
        bool space;
        bool r;
        virtual void update() = 0;
};

#ifdef SDL
#include "input.SDL.h"
#endif

#endif
