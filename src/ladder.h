#ifndef GUARD_LADDER_H
#define GUARD_LADDER_H

#include "entity.h"

#include <string>

using std::string;

class Ladder : public Entity {
    public:
        Ladder(int x, int y);
        virtual string sprite() override;
        bool base_ladder;
};

#endif
