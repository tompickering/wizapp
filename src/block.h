#ifndef GUARD_BLOCK_H
#define GUARD_BLOCK_H

#include "entity.h"

#include <string>

using std::string;

class Block : public Entity {
    public:
        Block(int x, int y);
        bool push(bool left);
        virtual string sprite() override;
};

#endif
