#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include "anim_sequence.h"

#include <vector>

#include "scene.h"
#include "anim/animation.h"

#define WINDS_TIME 5.f
#define FANTASIA_TIME 4.f
#define PRESENTS_TIME 4.2f
#define NOTHING_TIME  1.5f
#define KIDDING_TIME  0.5f
#define BAND_TIME     13.5f
#define TITLE1_TIME   5.f
#define TITLE2_TIME   5.f
#define END_TIME      5.f


using std::vector;

class Intro : public AnimSequence {
    public:
        Intro();
        virtual void update(float delta_time) override;
    protected:
        virtual void next_event() override;
    private:
        float skip_duration;
        bool skip_pressed;
        float skip_time;
        void winds();
        void fantasia();
        void presents();
        void nothing();
        void kidding();
        void band();
        void title1();
        void title2();
        void end();

        void next_scene();
        void next_credits();

        bool winds_done;
        bool fantasia_done;
        bool presents_done;
        bool nothing_done;
        bool kidding_done;
        bool band_done;
        bool title1_done;
        unsigned int scene_done;
        unsigned int credits_done;
        bool title2_done;
        bool end_done;
};

#endif
