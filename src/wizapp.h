#ifndef GUARD_WIZAPP_H
#define GUARD_WIZAPP_H

#include "scene.h"

class WizApp {
    public:
        int run();
    private:
        void init();
        void play_scene(SceneRef scene);
};

#endif
