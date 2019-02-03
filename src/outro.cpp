#include "outro.h"

#include <vector>

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

    switch (world_idx) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }

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

