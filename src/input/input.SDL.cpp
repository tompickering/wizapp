#include "input.SDL.h"

#include <SDL2/SDL.h>

#include "../shared.h"

void SDLInputManager::update() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN && !e.key.repeat) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    up = true;
                    break;
                case SDLK_DOWN:
                    down = true;
                    break;
                case SDLK_LEFT:
                    left = true;
                    break;
                case SDLK_RIGHT:
                    right = true;
                    break;
                case SDLK_SPACE:
                    space = true;
                    break;
                case SDLK_r:
                    r = true;
                    break;
                case SDLK_q:
                    q = true;
                    break;
                case SDLK_LSHIFT:
                    lshift = true;
                    break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    up = false;
                    break;
                case SDLK_DOWN:
                    down = false;
                    break;
                case SDLK_LEFT:
                    left = false;
                    break;
                case SDLK_RIGHT:
                    right = false;
                    break;
                case SDLK_SPACE:
                    space = false;
                    break;
                case SDLK_r:
                    r = false;
                    break;
                case SDLK_q:
                    q = false;
                    break;
                case SDLK_LSHIFT:
                    lshift = false;
                    break;
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&click_pos.x, &click_pos.y);
            logger.debug("Click: " + std::to_string(click_pos.x) + "," + std::to_string(click_pos.y));
        }
    }
}
