#ifndef GUARD_BREAKABLE_H
#define GUARD_BREAKABLE_H

#include "entity.h"

#include <string>

#include "anim/animation.h"

using std::string;

class Breakable : public Entity {
    public:
        Breakable(int x, int y);
        virtual ~Breakable();
        bool break_block();
        virtual void update(float delta_time) override;
        virtual void reset() override;
        virtual string sprite() override;
    private:
        bool breaking;
        Animation *anim_idle;
        Animation *anim_break;
};

#endif
