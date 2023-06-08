#include "main_menu.h"

#include <string>
#include <vector>

#include "../scene.h"
#include "../draw/draw.h"
#include "../save/save.h"
#include "../anim/animation.h"
#include "../anim/animation_looping.h"

#include "../shared.h"

using std::string;
using std::vector;

MainMenu::MainMenu() {
    round_select = nullptr;
    rs_bg = nullptr;
    rs_back = nullptr;
    reset();
}

void MainMenu::next_event() {
    AnimClick click_data = draw_manager.read_clicked_animation();
    Animation *clicked = click_data.anim;
    if (!start_done) {
        start_done = true;
        start_main();
    } else if (clicked) {
        if (clicked == round_select) {
            start_round_select();
        } else if (clicked == rs_bg) {
            int unlocked_world = savegame.world();
            float rx = click_data.rel_x;
            int world = 3;
            if (rx <= 0.2)
                world = 0;
            else if (rx <= 0.38)
                world = 1;
            else if (rx <= 0.6)
                world = 4;
            else if (rx <= 0.77)
                world = 2;

            if (world <= unlocked_world)
                open_world(world);
        } else if (clicked == rs_back) {
            start_main();
        } else {
            for (unsigned int i = 0; i < level_icons.size(); ++i) {
                if (clicked == level_icons.at(i)) {
                    unsigned int level_no = 1 + (world_open * 20) + i;
                    if (savegame.level_state(level_no) != Locked) {
                        level_to_start = level_no;
                        complete = true;
                    }
                }
            }
        }
    }
}

unsigned int MainMenu::get_level_to_start() {
    return level_to_start;
}

void MainMenu::clear_anims() {
    AnimSequence::clear_anims();
    /* Everything in level_icons is in current_anims and will have been deleted */
    level_icons.clear();
}

void MainMenu::reset() {
    action = MA_None;
    complete = false;
    start_done = false;
    fade_music_done = false;
}

void MainMenu::start_main() {
    clear_anims();
    string w = std::to_string(savegame.world() + 1);
    audio_manager.play_music("assets/audio/DWARVES.ogg", false, true);
    Animation *background = new Animation(.5f, .5f, "assets/img/menu/world" + w, 1, 1.f);
    round_select = new AnimationLooping(.5f, .5f, "assets/img/menu/round", 10, 1.f);
    current_anims.push_back(background);
    current_anims.push_back(round_select);
}

void MainMenu::start_round_select() {
    clear_anims();
    unsigned int world = savegame.world();
    audio_manager.play_music("assets/audio/WINDS.ogg", false, true);
    rs_bg = new Animation(.5f, .5f, "assets/img/menu/hub", 1, 1.f);
    rs_back = new Animation(.75f, .85f, "assets/img/menu/goback", 1, 1.f);
    current_anims.push_back(rs_bg);
    current_anims.push_back(rs_back);

    /* No-entry signs */
    Animation *ne;
    switch (world) {
        case 0:
            /* Sun */
            ne = new Animation(.32f, .55f, "assets/img/menu/notyet", 1, 1.f);
            current_anims.push_back(ne);
        case 1:
            /* Jazz */
            ne = new Animation(.68f, .55f, "assets/img/menu/notyet", 1, 1.f);
            current_anims.push_back(ne);
        case 2:
            /* Star */
            ne = new Animation(.85f, .53f, "assets/img/menu/notyet", 1, 1.f);
            current_anims.push_back(ne);
        case 3:
            /* Fantasia */
            ne = new Animation(.48f, .35f, "assets/img/menu/notyet", 1, 1.f);
            current_anims.push_back(ne);
    }
}

void MainMenu::open_world(unsigned int world) {
    world_open = world;
    Animation *s;
    for (int i = 0; i < 20; ++i) {
        unsigned int level_no = 1 + (world_open * 20) + i;

        float x = 0.10f + 0.20f * (i % 5);
        float y = 0.20f + 0.16f * (i / 5);
        s = new Animation(x, y, "assets/img/menu/select/level", 1, 1.f);
        level_icons.push_back(s);
        current_anims.push_back(s);

        LevelState lstate = savegame.level_state(level_no);
        if (lstate == Completed) {
            s = new Animation(x, y, "assets/img/menu/select/tick", 1, 1.f);
            s->clickable = false;
            current_anims.push_back(s);
        } else if (lstate == Locked) {
            s = new Animation(x, y, "assets/img/menu/select/cross", 1, 1.f);
            s->clickable = false;
            current_anims.push_back(s);
        }
    }
}

void MainMenu::fade_music() {
    audio_manager.fade_out(3000);
}
