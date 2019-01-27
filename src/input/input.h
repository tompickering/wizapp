#ifndef GUARD_INPUTMANAGER_H
#define GUARD_INPUTMANAGER_H

enum Input {
    Up,
    Down,
    Left,
    Right,
    Space,
    R,
    Q,
    LShift,
};

typedef struct {
    int x;
    int y;
} ClickPos;

class InputManager {
    public:
        InputManager();
        bool read(Input, bool reset);
        ClickPos read_click();
    protected:
        bool up;
        bool down;
        bool left;
        bool right;
        bool space;
        bool lshift;
        bool r;
        bool q;
        ClickPos click_pos;
        virtual void update() = 0;
};

#ifdef SDL
#include "input.SDL.h"
#endif

#endif
