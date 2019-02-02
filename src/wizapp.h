#ifndef GUARD_WIZAPP_H
#define GUARD_WIZAPP_H

#include <string>

#include "event.h"
#include "intro.h"
#include "outro.h"
#include "scene.h"
#include "level.h"
#include "menu/main_menu.h"

using std::string;

enum GameState {
    GS_Intro,
    GS_Menu,
    GS_RoundSelect,
    GS_Event,
    GS_StartLevel,
    GS_EndLevel,
    GS_Level,
    GS_StartLevScene,
    GS_EndLevScene,
    GS_Outro,
};

class WizApp {
    public:
        WizApp();
        int run();
    private:
        GameState state;
        Intro intro;
        Outro outro;
        Event event;
        Scene *scene;
        MainMenu menu;
        SceneRef scene_just_played;
        Level *level;
        unsigned int level_no;
        float next_level_pause;
        float fade_time;
        int level_just_completed;
        void init();
        void update(float delta_time);
        void set_state(GameState new_state);
        string state2str(GameState state);
        float speed;
};

#endif
