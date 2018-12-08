#ifndef GUARD_WIZAPP_H
#define GUARD_WIZAPP_H

#include <string>

#include "event.h"
#include "intro.h"
#include "scene.h"
#include "level.h"

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
};

class WizApp {
    public:
        WizApp();
        int run();
    private:
        GameState state;
        Intro intro;
        Event event;
        Scene *scene;
        SceneRef scene_just_played;
        Level *level;
        int level_no;
        int level_just_completed;
        void init();
        void update(float delta_time);
        void set_state(GameState new_state);
        string state2str(GameState state);
        float speed;
};

#endif
