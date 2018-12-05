#ifndef GUARD_DRAW_SDL_CPP
#define GUARD_DRAW_SDL_CPP

#include "draw.SDL.h"

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../collectable.h"
#include "../scene.h"
#include "../anim/animation.h"

#include "../shared.h"

using std::string;

void SDLDrawManager::init() {
    logger.info("DrawManager Init...");
    this->current_scene = None;
    this->win = SDL_CreateWindow(PROG_NAME,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);
    this->surf = SDL_GetWindowSurface(this->win);
    this->background_spr = NULL;
    this->background_path = string("");

    this->scene_font = TTF_OpenFont("font/Tangerine-Regular.ttf", 36);
    if (!this->scene_font) {
        logger.fatal(TTF_GetError());
    }

    SDL_FillRect(this->surf, NULL, SDL_MapRGB(this->surf->format, 0x0, 0x0, 0x0));
}

void SDLDrawManager::update(Level *level) {
    SDL_Rect surf_rect;
    Entity *ent;
    int blocksz_w = SCREEN_WIDTH/level->block_w;
    int blocksz_h = SCREEN_HEIGHT/level->block_h;
    string img_path;
    this->current_scene = None;

    if (!this->background_spr || this->background_path != level->get_background()) {
        this->background_path = level->get_background();
        this->background_spr = (void*) IMG_Load(this->background_path.c_str());
    }

    SDL_BlitSurface((SDL_Surface*) this->background_spr, NULL, this->surf, NULL);
    for (unsigned int i = 0; i < level->entities.size(); ++i) {
        ent = level->entities[i];
        if (ent->ignore)
            continue;
        surf_rect = {int((ent->real_x + ent->x_draw_offset) * (float) blocksz_w),
                     int((ent->real_y + ent->y_draw_offset) * (float) blocksz_h),
                     blocksz_w,
                     blocksz_h};
        img_path = ent->sprite();
        SDL_Surface *spr_surf = (SDL_Surface*) this->get_sprite_data(img_path);
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, this->surf, &surf_rect);
        } else {
            SDL_FillRect(this->surf, &surf_rect, SDL_MapRGB(this->surf->format,
                                                            ent->r, ent->g, ent->b));
        }
    }
    SDL_UpdateWindowSurface(this->win);
}

void SDLDrawManager::update(Scene *scene) {
    if (scene->scene_ref != this->current_scene) {
        /* Clear screen first */
        SDL_FillRect(this->surf, NULL, SDL_MapRGB(this->surf->format, 0x0, 0x0, 0x0));
        this->current_scene = scene->scene_ref;
        SDL_Surface *spr_surf = (SDL_Surface*) this->get_sprite_data(scene->image_path());
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, this->surf, NULL);
        }
    }

    /* Update text */
    SDL_Color white = {255, 255, 255};
    SDL_Surface *msg_surf = TTF_RenderText_Solid((TTF_Font*) this->scene_font,
                                                 scene->current_text().c_str(),
                                                 white);

    SDL_Rect back_rect;
    back_rect.x = 0;
    back_rect.y = 440;
    back_rect.w = SCREEN_WIDTH - 1;
    back_rect.h = 200;

    SDL_Rect msg_rect;
    msg_rect.x = 10;
    msg_rect.y = 448;
    msg_rect.w = SCREEN_WIDTH - 20;
    msg_rect.h = 100;

    SDL_FillRect(this->surf, &back_rect, SDL_MapRGB(this->surf->format, 0x0, 0x0, 0x0));
    SDL_BlitSurface(msg_surf, NULL, this->surf, &msg_rect);
    SDL_UpdateWindowSurface(this->win);
}

void SDLDrawManager::update(vector<Animation*> anims) {
    Animation *anim;
    SDL_FillRect(this->surf, NULL, SDL_MapRGB(this->surf->format, 0x0, 0x0, 0x0));
    for (unsigned int x = 0; x < anims.size(); ++x) {
        anim = anims.at(x);
        SDL_Surface *spr_surf = (SDL_Surface*) this->get_sprite_data(anim->sprite());
        SDL_Rect surf_rect = {int(anim->get_x() * (float) SCREEN_WIDTH - spr_surf->w / 2.f),
                              int(anim->get_y() * (float) SCREEN_HEIGHT - spr_surf->h / 2.f),
                              spr_surf->w,
                              spr_surf->h};
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, this->surf, &surf_rect);
        }
    }
    SDL_UpdateWindowSurface(this->win);
}

void* SDLDrawManager::get_sprite_data(string img_path) {
    if (img_path != "") {
        if (!this->sprite_data.count(img_path)) {
            this->sprite_data[img_path] = (void*) IMG_Load(img_path.c_str());
            return this->sprite_data[img_path];
        }
        return this->sprite_data[img_path];
    }
    return NULL;
}

#endif