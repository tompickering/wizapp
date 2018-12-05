#ifndef GUARD_STATIC_BLOCK_H
#define GUARD_STATIC_BLOCK_H

#include "entity.h"

#include <string>

using std::string;

class StaticBlock : public Entity {
    public:
        StaticBlock(int x, int y);
        virtual string sprite() override;
};

#endif
