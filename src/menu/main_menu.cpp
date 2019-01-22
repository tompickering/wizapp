#include "main_menu.h"

#include <string>
#include <vector>

#include "../scene.h"
#include "../anim/animation.h"
#include "../anim/animation_looping.h"

#include "../shared.h"

using std::string;
using std::vector;

MainMenu::MainMenu() {
    action = MA_None;
    round_select = NULL;
    start_done = false;
    fade_music_done = false;
}

void MainMenu::next_event() {
    if (!start_done) {
        start_done = true;
        start();
    } else if (draw_manager.read_clicked_animation() == round_select) {
        this->complete = true;
    }
}

void MainMenu::start() {
    string w = std::to_string(savegame.world() + 1);
    audio_manager.play_music("assets/audio/DWARVES.ogg", false, true);
    Animation *background = new Animation(.5f, .5f, "assets/img/menu/world" + w, 1, 1.f);
    round_select = new AnimationLooping(.5f, .5f, "assets/img/menu/round", 10, 1.f);
    current_anims.push_back(background);
    current_anims.push_back(round_select);
}

void MainMenu::fade_music() {
    //this->clear_anims();
    audio_manager.fade_out(3000);
}
