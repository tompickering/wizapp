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
                    this->up = true;
                    break;
                case SDLK_DOWN:
                    this->down = true;
                    break;
                case SDLK_LEFT:
                    this->left = true;
                    break;
                case SDLK_RIGHT:
                    this->right = true;
                    break;
                case SDLK_SPACE:
                    this->space = true;
                    break;
                case SDLK_r:
                    this->r = true;
                    break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_UP:
                    this->up = false;
                    break;
                case SDLK_DOWN:
                    this->down = false;
                    break;
                case SDLK_LEFT:
                    this->left = false;
                    break;
                case SDLK_RIGHT:
                    this->right = false;
                    break;
                case SDLK_SPACE:
                    this->space = false;
                    break;
                case SDLK_r:
                    this->r = false;
                    break;
            }
        }
    }
}
