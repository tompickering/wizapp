#include "intro.h"

#include <vector>

#include "input/input.h"

#include "scene.h"
#include "anim/animation.h"
#include "anim/animation_boomerang.h"
#include "anim/animation_looping.h"

#include "shared.h"

using std::vector;

Intro::Intro() {
    skip_time = 0.f;
    skip_pressed = false;
    skip_duration = 3.f;
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
    } else if (!scene1_done) {
        scene1();
        scene1_done = true;
    } else if (!scene2_done) {
        scene2();
        scene2_done = true;
    } else if (!scene3_done) {
        scene3();
        scene3_done = true;
    } else if (!scene4_done) {
        scene4();
        scene4_done = true;
    } else if (!scene5_done) {
        scene5();
        scene5_done = true;
    } else if (!scene6_done) {
        scene6();
        scene6_done = true;
    } else if (!scene7_done) {
        scene7();
        scene7_done = true;
    } else if (!scene8_done) {
        scene8();
        scene8_done = true;
    } else if (!title2_done) {
        title2();
        title2_done = true;
        next_event_countdown = TITLE2_TIME;
    } else if (!end_done) {
        end();
        end_done = true;
        next_event_countdown = END_TIME;
    }
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

void Intro::scene1() {
    clear_anims();
    current_scene = new Scene(Intro1);
}

// Programmming: Bill Kotsias

void Intro::scene2() {
    clear_anims();
    current_scene = new Scene(Intro2);
}

// Additional Code: Kostas Proitsakis (GrimAce)

void Intro::scene3() {
    clear_anims();
    current_scene = new Scene(Intro3);
}

// 2D Art: Spiros Vergos

void Intro::scene4() {
    clear_anims();
    current_scene = new Scene(Intro4);
}

// 3D Modelling: Bill Kotsias

void Intro::scene5() {
    clear_anims();
    current_scene = new Scene(Intro5);
}

// Special Effects: Spiros Vergos

void Intro::scene6() {
    clear_anims();
    current_scene = new Scene(Intro6);
}

// Music & SFX: Bill Kotsias

void Intro::scene7() {
    clear_anims();
    current_scene = new Scene(Intro7);
}

// Sound Player: Digitial Symphony - BASS

void Intro::scene8() {
    clear_anims();
    current_scene = new Scene(Intro8);
}

// AutoVCache Module: T.Karwoth

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

