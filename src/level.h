#ifndef GUARD_LEVEL_H
#define GUARD_LEVEL_H

#include <string>
#include <vector>

#include "entity.h"
#include "character.h"

using std::string;
using std::vector;

enum Theme {
    Valley,
    Industrial,
    Sun,
    Snowman,
    Jazz,
    Stereo,
    Star,
    Alien,
    Fantasia1,
    Fantasia2
};

enum LevelSet {
    Original,
    Test,
};

class Level {
    public:
        Level(LevelSet set, unsigned int number);
        unsigned int number;
        Theme theme;
        void clear();
        void update(float delta_time);
        vector<Entity*> entities;
        Character *wizard = nullptr;
        Character *blob = nullptr;
        int block_w;
        int block_h;
        bool on_ladder(int x, int y);
        bool is_empty(int x, int y);
        bool is_navigable(int x, int y, int from_x, int from_y, bool updown);
        bool complete;
        Entity* at(int x, int y);
        Collectable* attempt_collect(int x, int y);
        bool stable();
        Character *active_character;
        bool switch_character();
        void update_falling();
        void reset();
        string get_theme_base();
        string theme_base;
        string file_path;
        void load();
        string get_background();
        string get_track();
};

#endif
