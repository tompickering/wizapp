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
};

class MainMenu : public AnimSequence {
    public:
        MainMenu();
    protected:
        virtual void next_event() override;
    private:
        MenuAction action;
        Animation *round_select;
        void start();
        void fade_music();
        bool start_done;
        bool fade_music_done;
};

#endif
