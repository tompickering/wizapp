#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "../anim_sequence.h"

#include <vector>

#include "../anim/animation.h"

#define START_TIME 0.f


using std::vector;

enum MenuAction {
    MA_None,
    MA_RoundSelect,
    MA_StartLevel,
};

class MainMenu : public AnimSequence {
    public:
        MainMenu();
        void reset();
        unsigned int get_level_to_start();
    protected:
        virtual void next_event() override;
    private:
        virtual void clear_anims() override;
        MenuAction action;
        unsigned int level_to_start;
        unsigned int world_open;
        Animation *round_select;
        Animation *rs_bg;
        Animation *rs_back;
        /* Level squares */
        vector<Animation*> level_icons;
        void start_main();
        void start_round_select();
        void start_round_select_world(unsigned int);
        void open_world(unsigned int world);
        void fade_music();
        bool start_done;
        bool fade_music_done;
};

#endif
