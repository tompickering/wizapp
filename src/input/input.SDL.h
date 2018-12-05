#ifndef GUARD_INPUT_SDL_H
#define GUARD_INPUT_SDL_H

#include "input.h"

class SDLInputManager : public InputManager {
    public:
        virtual void update() override;
};

#endif
