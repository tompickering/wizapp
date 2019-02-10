#include "intro.h"

#include <vector>

#include "input/input.h"

#include "scene.h"
#include "anim/animation.h"
#include "anim/animation_boomerang.h"
#include "anim/animation_looping.h"
#include "anim/animation_text.h"

#include "shared.h"

using std::vector;

Intro::Intro() {
    skip_time = 0.f;
    skip_pressed = false;
    skip_duration = 3.f;
    scene_done = 0;
    credits_done = 0;
}

void Intro::next_event() {
    if (!winds_done) {
        winds();
        winds_done = true;
        next_event_countdown = WINDS_TIME;
    } else if (!fantasia_done) {
        fantasia();
        fantasia_done = true;
        next_event_countdown = FANTASIA_TIME;
    } else if (!presents_done) {
        presents();
        presents_done = true;
        next_event_countdown = PRESENTS_TIME;
    } else if (!nothing_done) {
        nothing();
        nothing_done = true;
        next_event_countdown = NOTHING_TIME;
    } else if (!kidding_done) {
        kidding();
        kidding_done = true;
        next_event_countdown = KIDDING_TIME;
    } else if (!band_done) {
        band();
        band_done = true;
        next_event_countdown = BAND_TIME;
    } else if (!title1_done) {
        title1();
        title1_done = true;
        next_event_countdown = TITLE1_TIME;
    } else if (scene_done == 8 && credits_done == 7) {
        if (!title2_done) {
            title2();
            title2_done = true;
            next_event_countdown = TITLE2_TIME;
        } else if (!end_done) {
            end();
            end_done = true;
            next_event_countdown = END_TIME;
        }
    } else {
        if (scene_done == credits_done) {
            next_scene();
        } else {
            next_credits();
        }
    }
}

void Intro::next_scene() {
    clear_anims();
    SceneRef to_play = (SceneRef) ((int)(Intro1) + scene_done);
    current_scene = new Scene(to_play);
    scene_done++;
}

void Intro::next_credits() {
    clear_anims();
    // Original game:
    // Programmming: Bill Kotsias
    // Additional Code: Kostas Proitsakis (GrimAce)
    // 2D Art: Spiros Vergos
    // 3D Modelling: Bill Kotsias
    // Special Effects: Spiros Vergos
    // Music & SFX: Bill Kotsias
    // Sound Player: Digitial Symphony - BASS
    // AutoVCache Module: T.Karwoth

    float h0 = .45f;
    float h1 = .55f;

    switch (credits_done) {
        case 0:
            current_anims.push_back(
                new AnimationText(.5f, h0, "Programming (Original)", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Bill Kotsias", 0, CREDITS_TIME));
            break;
        case 1:
            current_anims.push_back(
                new AnimationText(.5f, h0, "Programming (2018 Remake)", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Tom Pickering", 0, CREDITS_TIME));
            break;
        case 2:
            current_anims.push_back(
                new AnimationText(.5f, h0, "2D Art", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Spiros Vergos", 0, CREDITS_TIME));
            break;
        case 3:
            current_anims.push_back(
                new AnimationText(.5f, h0, "3D Modelling", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Bill Kotsias", 0, CREDITS_TIME));
            break;
        case 4:
            current_anims.push_back(
                new AnimationText(.5f, h0, "Special Effects", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Spiros Vergos", 0, CREDITS_TIME));
            break;
        case 5:
            current_anims.push_back(
                new AnimationText(.5f, h0, "Music & SFX", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, h1, "Bill Kotsias", 0, CREDITS_TIME));
            break;
        case 6:
            current_anims.push_back(
                new AnimationText(.5f, .4f, "Additional Thanks", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, .5f, "Kostas Proitsakis", 0, CREDITS_TIME));
            current_anims.push_back(
                new AnimationText(.5f, .6f, "T. Karwoth", 0, CREDITS_TIME));
            break;
        default:
            break;
    }

    next_event_countdown = CREDITS_TIME;
    credits_done++;
}

void Intro::update(float delta_time) {
    if (input_manager.read(Space, true)) {
        skip_pressed = true;
        audio_manager.fade_out((int)(skip_duration * 1000.f * .8f));
    }

    if (skip_pressed) {
        skip_time += delta_time;
        if (skip_time > skip_duration) {
            complete = true;
        }
    }

    AnimSequence::update(delta_time);
}

void Intro::winds() {
    audio_manager.play_music("assets/audio/intro_winds.ogg", false, false);
}

void Intro::fantasia() {
    current_anims.push_back(
        new AnimationBoomerang(.5f, .4f, "assets/img/intro/fantasia", 15, 1.f));
}

void Intro::presents() {
    current_anims.push_back(
        new AnimationBoomerang(.5f, .9f, "assets/img/intro/presents", 19, 1.f));
}

void Intro::nothing() {
    clear_anims();
    audio_manager.play_music("assets/audio/intro_nothing.ogg", true, false);
    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/nothing", 1, 1.f));
}

void Intro::kidding() {
    logger.info("(Just kidding!)");
}

void Intro::band() {
    clear_anims();
    audio_manager.play_music("assets/audio/intro_band.ogg", true, true);
    Animation *band1 = new AnimationBoomerang(0.8f, .5f, "assets/img/intro/band2/trumpet", 9, 0.3f);
    Animation *band2 = new AnimationBoomerang(0.9f, .5f, "assets/img/intro/band2/drum", 9, 0.3f);
    Animation *band3 = new AnimationBoomerang(1.0f, .5f, "assets/img/intro/band2/last", 9, 0.3f);
    Animation *wiz = new Animation(1.3f, .425f, "assets/img/intro/wizard01", 1, 1.f);
    Animation *app = new Animation(1.3f, .575f, "assets/img/intro/apprentice01", 1, 1.f);
    band1->set_motion(-0.8, 0.f, BAND_TIME);
    band2->set_motion(-0.8, 0.f, BAND_TIME);
    band3->set_motion(-0.8, 0.f, BAND_TIME);
    wiz->set_motion(-0.8, 0.f, BAND_TIME);
    app->set_motion(-0.8, 0.f, BAND_TIME);
    current_anims.push_back(band1);
    current_anims.push_back(band2);
    current_anims.push_back(band3);
    current_anims.push_back(wiz);
    current_anims.push_back(app);
}

void Intro::title1() {
    clear_anims();
    audio_manager.play_music("assets/audio/intro_wizapp.ogg", true, false);
    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/wizback", 1, 1.f));
    current_anims.push_back(
        new Animation(.5f, .35f, "assets/img/intro/wizard06", 1, 1.f));
    current_anims.push_back(
        new Animation(.5f, .65f, "assets/img/intro/apprentice06", 1, 1.f));
}

void Intro::title2() {
    clear_anims();
    current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/wizback", 1, 1.f));
    current_anims.push_back(
        new Animation(.5f, .35f, "assets/img/intro/wizard06", 1, 1.f));
    current_anims.push_back(
        new Animation(.5f, .65f, "assets/img/intro/apprentice06", 1, 1.f));
}

void Intro::end() {
}

