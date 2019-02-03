#ifndef GUARD_COLLECTABLE_H
#define GUARD_COLLECTABLE_H

#include "entity.h"

#include <string>

#include "anim/animation.h"

using std::string;

enum CollectableType {
    Flower,
    Mushroom,
    FMushroom,
    Atom,
    Energy,
    Shadow,
    FShadow,
    Note1,
    Note2,
    Clef,
    Note3,
    Star1,
    FStar1,
    Star2,
    FStar2,
    Diamond,
    FDiamond,
    Unknown,
    FUnknown
};

class Collectable : public Entity {
    public:
        Collectable(int x, int y, bool flying);
        ~Collectable();
        void collect();
        bool collected; // Collection event has been triggered
        virtual void update(float delta_time) override;
        virtual void reset() override;
        virtual string sprite() override;
        CollectableType ctype;
        float get_shield();

        static Animation* make_anim(CollectableType type);
    private:
        Animation *anim_idle;
        Animation *anim_collect;
        float shield;
};

#endif
