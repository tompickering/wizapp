#ifndef GUARD_CONTROLLER_H
#define GUARD_CONTROLLER_H

class Controller {
    public:
        bool left;
        bool right;
        bool up;
        bool down;
        bool switch_character;
        bool reset;
        void update();
};

#endif
