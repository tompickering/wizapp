#ifndef GUARD_DRAW_SDL_H
#define GUARD_DRAW_SDL_H

#include "draw.h"

#include <vector>

#include <SDL2/SDL.h>

#include "../level.h"
#include "../scene.h"
#include "../anim/animation.h"

class SDLDrawManager : public DrawManager {
    public:
        virtual void init() override;
        virtual void update(Level *level) override;
        virtual void update(Scene *scene) override;
        virtual void update(vector<Animation*> anims) override;
        virtual AnimClick read_clicked_animation();
    protected:
        virtual void* get_sprite_data(string) override;
    private:
        SDL_Window *win;
        SDL_Surface *surf;
};

#endif
