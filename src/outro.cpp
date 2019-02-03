#include "outro.h"

#include <vector>

#include "collectable.h"
#include "anim/animation.h"
#include "anim/animation_looping.h"
#include "anim/animation_sequence.h"

#include "shared.h"

using std::vector;

Outro::Outro() {
    on_world = 0;
    world_end_required = false;
    start_done = false;
    fantasia_done = false;
    end_done = false;
}

void Outro::next_event() {
    if (world_end_required) {
        world_end(on_world++);
        next_event_countdown = MUSIC_FADE_TIME + 1.f;
        world_end_required = false;
    } else if (!start_done) {
        start_done = true;
        start();
        next_event_countdown = PORTAL_CLOSED_TIME;
    } else if (end_done) {
        complete = true;
    } else if (on_world >= 6) {
        end();
        next_event_countdown = BYE_TIME;
        end_done = true;
    } else {
        world(on_world);
        next_event_countdown = world_times[on_world];
        world_end_required = true;
    }
}

void Outro::portal_open() {
    current_anims.push_back(
        new Animation(.5f, PORTAL_Y, "assets/img/extro/portal/", 10, PORTAL_OPEN_TIME));
}

void Outro::start() {
    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/extro/back", 1, 1.f));
    current_anims.push_back(
        new Animation(.5f, PORTAL_Y, "assets/img/extro/portal/01", 1, 1.f));
}

void Outro::world(int world_idx) {
    clear_anims();

    /* World music */
    if (world_idx < 5) {
        int music_idx = (world_idx * 2) + 1;
        audio_manager.play_music(
                "assets/audio/TRACK" + std::to_string(music_idx) + ".ogg", true, false);
    } else {
        audio_manager.play_music("assets/audio/FAIRYTALE.ogg", true, false);
    }

    /* World background */
    current_anims.push_back(
        new Animation(.5f, PORTAL_Y + .02f,
            "assets/img/extro/KOSMOS" + std::to_string(world_idx + 1), 1, 1.f));

    vector<Animation*> anims;
    switch (world_idx) {
        case 0:
            anims.push_back(Collectable::make_anim(Flower));
            anims.push_back(Collectable::make_anim(Mushroom));
            anims.push_back(Collectable::make_anim(FMushroom));
            break;
        case 1:
            anims.push_back(Collectable::make_anim(Atom));
            anims.push_back(Collectable::make_anim(Energy));
            anims.push_back(Collectable::make_anim(Shadow));
            anims.push_back(Collectable::make_anim(FShadow));
            anims.push_back(new Animation("assets/img/extro/snowman", 1, 1.f));
            anims.push_back(new Animation("assets/img/extro/sun", 1, 1.f));
            break;
        case 2:
            anims.push_back(Collectable::make_anim(Note1));
            anims.push_back(Collectable::make_anim(Note2));
            anims.push_back(Collectable::make_anim(Clef));
            anims.push_back(Collectable::make_anim(Note3));
            anims.push_back(new Animation("assets/img/extro/stereo", 1, 1.f));
            anims.push_back(new Animation("assets/img/extro/sax", 1, 1.f));
            break;
        case 3:
            anims.push_back(Collectable::make_anim(Star1));
            anims.push_back(Collectable::make_anim(FStar1));
            anims.push_back(Collectable::make_anim(Star1));
            anims.push_back(Collectable::make_anim(FStar2));
            anims.push_back(new Animation("assets/img/extro/alieneer", 1, 1.f));
            break;
        case 4:
            anims.push_back(Collectable::make_anim(Diamond));
            anims.push_back(Collectable::make_anim(FDiamond));
            anims.push_back(Collectable::make_anim(Unknown));
            anims.push_back(Collectable::make_anim(FUnknown));
            anims.push_back(new Animation("assets/img/extro/frog", 1, 1.f));
            break;
        case 5:
            anims.push_back(new Animation("assets/img/extro/masters", 1, 1.f));
            anims.push_back(new Animation("assets/img/extro/devs", 1, 1.f));
            break;
    }

    float base_x = 1.f;
    float interval_x = 1.f;
    for (unsigned int i = 0; i < anims.size(); ++i) {
        anims.at(i)->x = base_x + interval_x * i;
        anims.at(i)->y = PORTAL_Y + .05f;
        anims.at(i)->set_motion(-1.f * anims.size(), 0.f, world_times[world_idx]);
    }

    current_anims.insert(current_anims.end(), anims.begin(), anims.end());

    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/extro/back", 1, 1.f));

    portal_open();
}

void Outro::world_end(int world_idx) {
    clear_anims();
    audio_manager.fade_out((int)(MUSIC_FADE_TIME * 1000.f));

    /* World background */
    current_anims.push_back(
        new Animation(.5f, PORTAL_Y + .02f,
            "assets/img/extro/KOSMOS" + std::to_string(world_idx + 1), 1, 1.f));

    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/extro/back", 1, 1.f));

    current_anims.push_back(
        new Animation(.5f, PORTAL_Y, "assets/img/extro/portal/", 10, PORTAL_SHUT_TIME, true));
}

void Outro::end() {
    clear_anims();
    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/extro/bye", 1, 1.f));
}

