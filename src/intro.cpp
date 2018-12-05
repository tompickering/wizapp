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
    this->skip_time = 0.f;
    this->skip_pressed = false;
    this->skip_duration = 3.f;
}

void Intro::next_event() {
    if (!this->winds_done) {
        this->winds();
        this->winds_done = true;
        this->next_event_countdown = WINDS_TIME;
    } else if (!this->fantasia_done) {
        this->fantasia();
        this->fantasia_done = true;
        this->next_event_countdown = FANTASIA_TIME;
    } else if (!this->presents_done) {
        this->presents();
        this->presents_done = true;
        this->next_event_countdown = PRESENTS_TIME;
    } else if (!this->nothing_done) {
        this->nothing();
        this->nothing_done = true;
        this->next_event_countdown = NOTHING_TIME;
    } else if (!this->kidding_done) {
        this->kidding();
        this->kidding_done = true;
        this->next_event_countdown = KIDDING_TIME;
    } else if (!this->band_done) {
        this->band();
        this->band_done = true;
        this->next_event_countdown = BAND_TIME;
    } else if (!this->title1_done) {
        this->title1();
        this->title1_done = true;
        this->next_event_countdown = TITLE1_TIME;
    } else if (!this->scene1_done) {
        this->scene1();
        this->scene1_done = true;
    } else if (!this->scene2_done) {
        this->scene2();
        this->scene2_done = true;
    } else if (!this->scene3_done) {
        this->scene3();
        this->scene3_done = true;
    } else if (!this->scene4_done) {
        this->scene4();
        this->scene4_done = true;
    } else if (!this->scene5_done) {
        this->scene5();
        this->scene5_done = true;
    } else if (!this->scene6_done) {
        this->scene6();
        this->scene6_done = true;
    } else if (!this->scene7_done) {
        this->scene7();
        this->scene7_done = true;
    } else if (!this->scene8_done) {
        this->scene8();
        this->scene8_done = true;
    } else if (!this->title2_done) {
        this->title2();
        this->title2_done = true;
        this->next_event_countdown = TITLE2_TIME;
    } else if (!this->end_done) {
        this->end();
        this->end_done = true;
        this->next_event_countdown = END_TIME;
    }
}

void Intro::update(float delta_time) {
    if (input_manager.read(Space, true)) {
        this->skip_pressed = true;
        audio_manager.fade_out((int)(this->skip_duration * 1000.f * .8f));
    }

    if (this->skip_pressed) {
        this->skip_time += delta_time;
        if (this->skip_time > this->skip_duration) {
            this->complete = true;
        }
    }

    AnimSequence::update(delta_time);
}

void Intro::winds() {
    audio_manager.play_music("assets/audio/intro_winds.ogg", false, false);
}

void Intro::fantasia() {
    this->current_anims.push_back(
        new AnimationBoomerang(.5f, .4f, "assets/img/intro/fantasia", 15, 1.f));
}

void Intro::presents() {
    this->current_anims.push_back(
        new AnimationBoomerang(.5f, .9f, "assets/img/intro/presents", 19, 1.f));
}

void Intro::nothing() {
    this->clear_anims();
    audio_manager.play_music("assets/audio/intro_nothing.ogg", true, false);
    this->current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/nothing", 1, 1.f));
}

void Intro::kidding() {
    logger.info("(Just kidding!)");
}

void Intro::band() {
    this->clear_anims();
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
    this->current_anims.push_back(band1);
    this->current_anims.push_back(band2);
    this->current_anims.push_back(band3);
    this->current_anims.push_back(wiz);
    this->current_anims.push_back(app);
}

void Intro::title1() {
    this->clear_anims();
    audio_manager.play_music("assets/audio/intro_wizapp.ogg", true, false);
    this->current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/wizback", 1, 1.f));
    this->current_anims.push_back(
        new Animation(.5f, .35f, "assets/img/intro/wizard06", 1, 1.f));
    this->current_anims.push_back(
        new Animation(.5f, .65f, "assets/img/intro/apprentice06", 1, 1.f));
}

void Intro::scene1() {
    this->clear_anims();
    audio_manager.play_music("assets/audio/FAIRYTALE.ogg", true, false);
    this->current_scene = new Scene(Intro1);
}

// Programmming: Bill Kotsias

void Intro::scene2() {
    this->clear_anims();
    this->current_scene = new Scene(Intro2);
}

// Additional Code: Kostas Proitsakis (GrimAce)

void Intro::scene3() {
    this->clear_anims();
    this->current_scene = new Scene(Intro3);
}

// 2D Art: Spiros Vergos

void Intro::scene4() {
    this->clear_anims();
    this->current_scene = new Scene(Intro4);
}

// 3D Modelling: Bill Kotsias

void Intro::scene5() {
    this->clear_anims();
    this->current_scene = new Scene(Intro5);
}

// Special Effects: Spiros Vergos

void Intro::scene6() {
    this->clear_anims();
    this->current_scene = new Scene(Intro6);
}

// Music & SFX: Bill Kotsias

void Intro::scene7() {
    this->clear_anims();
    this->current_scene = new Scene(Intro7);
}

// Sound Player: Digitial Symphony - BASS

void Intro::scene8() {
    this->clear_anims();
    this->current_scene = new Scene(Intro8);
}

// AutoVCache Module: T.Karwoth

void Intro::title2() {
    this->clear_anims();
    this->current_anims.push_back(
        new Animation(.5f, .5f, "assets/img/intro/wizback", 1, 1.f));
    this->current_anims.push_back(
        new Animation(.5f, .35f, "assets/img/intro/wizard06", 1, 1.f));
    this->current_anims.push_back(
        new Animation(.5f, .65f, "assets/img/intro/apprentice06", 1, 1.f));
}

void Intro::end() {
}

