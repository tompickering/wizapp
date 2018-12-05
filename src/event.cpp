#include "event.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"
#include "anim/animation_boomerang.h"
#include "anim/animation_looping.h"

#include "shared.h"

using std::vector;

Event::Event() {
}

void Event::next_event() {
    if (!this->start_done) {
        this->start();
        this->start_done = true;
        this->next_event_countdown = 0.f;
    }
}

void Event::start() {
    audio_manager.play_music("assets/audio/CUTY.ogg", false, false);
    Animation *background = new Animation(.5f, .5f, "assets/img/themes/01/back/2", 1, 1.f);
    Animation *ladder1 = new Animation(.5f, 1.f/16.f, "assets/img/themes/01/ladder1", 1, 1.f);
    Animation *ladder2 = new Animation(.5f, 3.f/16.f, "assets/img/themes/01/ladder1", 1, 1.f);
    Animation *static1 = new Animation(.5f, 5.f/16.f, "assets/img/themes/01/static", 1, 1.f);
    Animation *static2 = new Animation(9.f / 22.f, 5.f/16.f, "assets/img/themes/01/static", 1, 1.f);
    Animation *block = new Animation(9.f / 22.f, 3.f/16.f, "assets/img/themes/01/block", 1, 1.f);
    Animation *blob = new AnimationBoomerang(-.1f, 15.f/16.f, "assets/img/blob/right", 5, 0.1f);
    Animation *baddie = new AnimationBoomerang(-.2f, 15.f/16.f, "assets/img/scene/baddie/right", 5, 0.1f);
    Animation *wiz = new AnimationLooping(.5f, -1.f/16.f, "assets/img/wiz/climb", 22, 0.5f);
    wiz->set_motion(0.f, 2.f/8.f, 1.f);
    blob->set_motion(1.5f, 0.f, 3.f);
    baddie->set_motion(1.5f, 0.f, 3.f);
    this->current_anims.push_back(background);
    this->current_anims.push_back(ladder1);
    this->current_anims.push_back(ladder2);
    this->current_anims.push_back(static1);
    this->current_anims.push_back(static2);
    this->current_anims.push_back(block);
    this->current_anims.push_back(blob);
    this->current_anims.push_back(baddie);
    this->current_anims.push_back(wiz);
}
