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

class DrawManager {
    public:
        virtual void init() = 0;
        virtual void update(Level *level) = 0;
        virtual void update(Scene *scene) = 0;
        virtual void update(vector<Animation*> anims) = 0;
    protected:
        map<string, void*> sprite_data;
        virtual void* get_sprite_data(string) = 0;
        void *background_spr;
        string background_path;
        SceneRef current_scene;
        void *scene_font;
};

#ifdef SDL
#include "draw.SDL.h"
#endif

#endif
