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
#include "../input/input.h"

#include "../shared.h"

using std::string;

void SDLDrawManager::init() {
    logger.info("DrawManager Init...");
    current_scene = None;
    clicked_animation = NULL;
    win = SDL_CreateWindow(PROG_NAME,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);
    surf = SDL_GetWindowSurface(win);
    background_spr = NULL;
    background_path = string("");

    scene_font = TTF_OpenFont("font/Tangerine-Regular.ttf", 36);
    if (!scene_font) {
        logger.fatal(TTF_GetError());
    }

    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0x0, 0x0, 0x0));
}

void SDLDrawManager::update(Level *level) {
    SDL_Rect surf_rect;
    Entity *ent;
    int blocksz_w = SCREEN_WIDTH/level->block_w;
    int blocksz_h = SCREEN_HEIGHT/level->block_h;
    string img_path;
    current_scene = None;

    if (!background_spr || background_path != level->get_background()) {
        background_path = level->get_background();
        background_spr = (void*) IMG_Load(background_path.c_str());
    }

    SDL_BlitSurface((SDL_Surface*) background_spr, NULL, surf, NULL);
    for (unsigned int i = 0; i < level->entities.size(); ++i) {
        ent = level->entities[i];
        if (ent->ignore)
            continue;
        img_path = ent->sprite();

        if (img_path == "")
            continue;

        surf_rect = {int((ent->real_x + ent->x_draw_offset) * (float) blocksz_w),
                     int((ent->real_y + ent->y_draw_offset) * (float) blocksz_h),
                     blocksz_w,
                     blocksz_h};

        SDL_Surface *spr_surf = (SDL_Surface*) get_sprite_data(img_path);
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, surf, &surf_rect);
        } else {
            SDL_FillRect(surf, &surf_rect, SDL_MapRGB(surf->format,
                                                            ent->r, ent->g, ent->b));
        }
    }
    SDL_UpdateWindowSurface(win);
}

void SDLDrawManager::update(Scene *scene) {
    if (scene->scene_ref != current_scene) {
        /* Clear screen first */
        SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0x0, 0x0, 0x0));
        current_scene = scene->scene_ref;
        SDL_Surface *spr_surf = (SDL_Surface*) get_sprite_data(scene->image_path());
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, surf, NULL);
        }
    }

    /* Update text */
    SDL_Color white = {255, 255, 255};
    SDL_Surface *msg_surf = TTF_RenderText_Solid((TTF_Font*) scene_font,
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

    SDL_FillRect(surf, &back_rect, SDL_MapRGB(surf->format, 0x0, 0x0, 0x0));
    SDL_BlitSurface(msg_surf, NULL, surf, &msg_rect);
    SDL_UpdateWindowSurface(win);
}

void SDLDrawManager::update(vector<Animation*> anims) {
    Animation *anim;
    ClickPos click_pos = input_manager.read_click();
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0x0, 0x0, 0x0));
    for (unsigned int x = 0; x < anims.size(); ++x) {
        anim = anims.at(x);
        SDL_Surface *spr_surf = (SDL_Surface*) get_sprite_data(anim->sprite());
        int draw_x = int(anim->get_x() * (float) SCREEN_WIDTH - spr_surf->w / 2.f);
        int draw_y = int(anim->get_y() * (float) SCREEN_HEIGHT - spr_surf->h / 2.f);
        SDL_Rect surf_rect = {draw_x,
                              draw_y,
                              spr_surf->w,
                              spr_surf->h};
        if (spr_surf) {
            SDL_BlitSurface(spr_surf, NULL, surf, &surf_rect);
            /* Check if this draw element has been clicked */
            if (click_pos.x >= draw_x && click_pos.x <= draw_x + spr_surf->w
                && click_pos.y >= draw_y && click_pos.y <= draw_y + spr_surf->h) {
                clicked_animation = anim;
            }
        }
    }
    SDL_UpdateWindowSurface(win);
}

void* SDLDrawManager::get_sprite_data(string img_path) {
    if (img_path != "") {
        if (!sprite_data.count(img_path)) {
            sprite_data[img_path] = (void*) IMG_Load(img_path.c_str());
            return sprite_data[img_path];
        }
        return sprite_data[img_path];
    }
    return NULL;
}

Animation* SDLDrawManager::read_clicked_animation() {
    Animation *result = clicked_animation;
    clicked_animation = NULL;
    return result;
}

#endif
