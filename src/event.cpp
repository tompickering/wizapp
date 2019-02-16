#include "event.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"
#include "anim/animation_looping.h"
#include "anim/animation_sequence.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::vector;

Event::Event() {
    start_done = false;
    scene_done = false;
    fade_music_done = false;
}

void Event::next_event() {
    if (!start_done) {
        start_done = true;
        start();
        next_event_countdown = 17.f;
    } else if (!scene_done) {
        scene_done = true;
        scene();
    } else if (!fade_music_done) {
        fade_music_done = true;
        fade_music();
        next_event_countdown = 4.f;
    } else {
        complete = true;
    }
}

void Event::start() {
    audio_manager.play_music("assets/audio/CUTY.ogg", false, true);
    Animation *background = new Animation(.5f, .5f, "assets/img/themes/01/back/2", 1, 1.f);
    Animation *ladder1 = new Animation(.5f, 1.f/16.f, "assets/img/themes/01/ladder1", 1, 1.f);
    Animation *ladder2 = new Animation(.5f, 3.f/16.f, "assets/img/themes/01/ladder1", 1, 1.f);
    Animation *static1 = new Animation(.5f, 5.f/16.f, "assets/img/themes/01/static", 1, 1.f);
    Animation *static2 = new Animation(9.f / 22.f, 5.f/16.f, "assets/img/themes/01/static", 1, 1.f);

    AnimationSequence *block = new AnimationSequence();
    Animation *block1 = new Animation(9.f / 22.f, 3.f/16.f, "assets/img/themes/01/block", 1, 8.7f);
    Animation *block2 = new Animation("assets/img/themes/01/block", 1, 0.1f);
    Animation *block3 = new Animation("assets/img/themes/01/block", 1, 0.2f);
    block2->set_motion(-1.f/11.f, 0, 0.1);
    block3->set_motion(0, 6.f/8.f, 0.2);
    block->add_animation(block1);
    block->add_animation(block2);
    block->add_animation(block3);

    Animation *blob1 = new AnimationBoomerang(-.1f, 15.f/16.f, "assets/img/blob/right", 5, 0.1f);
    Animation *baddie1 = new AnimationBoomerang(-.2f, 15.f/16.f, "assets/img/scene/baddie/right", 5, 0.1f);
    Animation *blob2 = new AnimationBoomerang(2.4f, 15.f/16.f, "assets/img/blob/left", 5, 0.1f);
    Animation *baddie2 = new AnimationBoomerang(2.5f, 15.f/16.f, "assets/img/scene/baddie/left", 5, 0.1f);

    AnimationSequence *blob3 = new AnimationSequence();
    AnimationBoomerang *blob3_1 = new AnimationBoomerang(-2.55f, 15.f/16.f, "assets/img/blob/right", 5, 0.1f);
    blob3_1->set_iterations(46);
    Animation *blob3_2 = new Animation("assets/img/blob/right", 1, 1.6f);
    Animation *blob3_3 = new Animation("assets/img/blob/turn", 6, 0.2f);
    Animation *blob3_4 = new Animation("assets/img/blob/left", 1, 1.0f);
    Animation *blob3_5 = new AnimationLooping("assets/img/blob/left_break", 11, 0.4f);
    blob3->add_animation(blob3_1);
    blob3->add_animation(blob3_2);
    blob3->add_animation(blob3_3);
    blob3->add_animation(blob3_4);
    blob3->add_animation(blob3_5);

    Animation *baddie3 = new AnimationBoomerang(-2.7f, 15.f/16.f, "assets/img/scene/baddie/right", 5, 0.1f);

    AnimationSequence *wiz = new AnimationSequence();
    Animation *wiz1 = new AnimationLooping(.5f, -1.f/16.f, "assets/img/wiz/climb", 22, 0.5f, 2);
    Animation *wiz2 = new Animation("assets/img/wiz/turn", 7, 0.2f, true);
    Animation *wiz3 = new Animation("assets/img/wiz/right", 1, 4.f);
    Animation *wiz4 = new Animation("assets/img/wiz/turn", 7, 0.2f);
    Animation *wiz5 = new Animation("assets/img/wiz/left", 1, 3.f);
    Animation *wiz6 = new Animation("assets/img/wiz/left_push", 12, 0.25f);
    wiz1->set_motion(0.f, 23.f/64.f, 1.5f);
    wiz->add_animation(wiz1);
    wiz->add_animation(wiz2);
    wiz->add_animation(wiz3);
    wiz->add_animation(wiz4);
    wiz->add_animation(wiz5);
    wiz->add_animation(wiz6);

    blob1->set_motion(1.5f, 0.f, 3.f);
    baddie1->set_motion(1.5f, 0.f, 3.f);
    blob2->set_motion(-3.5f, 0.f, 9.f);
    baddie2->set_motion(-3.5f, 0.f, 9.f);
    blob3_1->set_motion(3.f, 0.f, 9.f);
    baddie3->set_motion(3.f, 0.f, 9.f);

    current_anims.push_back(background);
    current_anims.push_back(ladder1);
    current_anims.push_back(ladder2);
    current_anims.push_back(static1);
    current_anims.push_back(static2);
    current_anims.push_back(blob1);
    current_anims.push_back(baddie1);
    current_anims.push_back(blob2);
    current_anims.push_back(baddie2);
    current_anims.push_back(blob3);
    current_anims.push_back(baddie3);
    current_anims.push_back(block);
    current_anims.push_back(wiz);
}

void Event::scene() {
    clear_anims();
    current_scene = new Scene(Friend);
}

void Event::fade_music() {
    clear_anims();
    audio_manager.fade_out(3000);
}
