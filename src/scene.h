#ifndef GUARD_SCENE_H
#define GUARD_SCENE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

enum SceneRef {
    Intro1,  // Masters: Welcome to your final tests...
    Intro2,  // Masters: During your quest...
    Intro3,  // Nohat: Take this magic wand...
    Intro4,  // Nohat: Use it wisely...
    Intro5,  // Hat: Take this magic hat...
    Intro6,  // Hat: But bear in mind...
    Intro7,  // Masters: At the end of your quest...
    Intro8,  // Masters: May the spirits...
    World1Start,
    World1Mid,
    World1End,
    World2Start,
    World2Mid,
    World2End,
    World3Start,
    World3Mid,
    World3End,
    World4Start,
    World4Mid,
    World4End,
    World5Start,
    World5Mid,
    World5End,
    Extro,
    Friend,
    None
};

class Scene {
    public:
        Scene(SceneRef);
        string music_path();
        string image_path();
        string current_text();
        float time;
        float char_time;
        bool complete;
        bool sentence_complete;
        SceneRef scene_ref;
        void update(float delta_time);
        vector<string> text;
        unsigned int sentence;
        float sentence_time;
};

#endif
