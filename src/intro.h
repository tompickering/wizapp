#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

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

enum IntroAnim {
    Fantasia,
    Presents,
    Nothing,
    Band1,
    Band2,
    Band3,
    WATrailer,
    WAFull,
};

class Intro {
    public:
        Intro();
        ~Intro();
        void update(float delta_time);
        bool complete;
    private:
        float skip_duration;
        bool skip_pressed;
        float skip_time;
        float time;
        float next_event_countdown;
        vector<Animation*> current_anims;
        Scene *current_scene;
        void clear_anims();
        void winds();
        void fantasia();
        void presents();
        void nothing();
        void kidding();
        void band();
        void title1();
        void scene1();
        // Programmming: Bill Kotsias
        void scene2();
        // Additional Code: Kostas Proitsakis (GrimAce)
        void scene3();
        // 2D Art: Spiros Vergos
        void scene4();
        // 3D Modelling: Bill Kotsias
        void scene5();
        // Special Effects: Spiros Vergos
        void scene6();
        // Music & SFX: Bill Kotsias
        void scene7();
        // Sound Player: Digitial Symphony - BASS
        void scene8();
        // AutoVCache Module: T.Karwoth
        void title2();
        void end();

        bool winds_done;
        bool fantasia_done;
        bool presents_done;
        bool nothing_done;
        bool kidding_done;
        bool band_done;
        bool title1_done;
        bool scene1_done;
        bool scene2_done;
        bool scene3_done;
        bool scene4_done;
        bool scene5_done;
        bool scene6_done;
        bool scene7_done;
        bool scene8_done;
        bool title2_done;
        bool end_done;
};

#endif
