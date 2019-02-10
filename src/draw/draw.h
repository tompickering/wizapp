#ifndef GUARD_DRAW_H
#define GUARD_DRAW_H

#include "../level.h"
#include "../scene.h"
#include "../anim/animation.h"

#include <string>
#include <map>
#include <vector>

#define SCREEN_WIDTH 640
//#define SCREEN_HEIGHT 512  // Original, with level scroll
#define SCREEN_HEIGHT 586

using std::string;
using std::map;

typedef struct {
    Animation *anim;
    float rel_x;
    float rel_y;
} AnimClick;

class DrawManager {
    public:
        virtual void init() = 0;
        virtual void update(Level *level) = 0;
        virtual void update(Scene *scene) = 0;
        virtual void update(vector<Animation*> anims) = 0;
        virtual AnimClick read_clicked_animation() = 0;
        virtual void hide_mouse(bool) = 0;
    protected:
        /* Using absolute co-ordinates */
        virtual void draw_text(string, int, int, int, int,
                               unsigned char, unsigned char, unsigned char) = 0;
        /* Using proportional (0-1) co-ordinates */
        virtual void draw_text(string, float, float, int, int,
                               unsigned char, unsigned char, unsigned char) = 0;
        map<string, void*> sprite_data;
        virtual void* get_sprite_data(string) = 0;
        void *background_spr;
        string background_path;
        SceneRef current_scene;
        void *scene_font;
        Animation *clicked_animation;
        float clicked_animation_rel_x;
        float clicked_animation_rel_y;
};

#ifdef SDL
#include "draw.SDL.h"
#endif

#endif
