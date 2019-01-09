#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include <string>
#include <vector>

#define LEVELS_BYTES 15

using std::vector;

enum LevelState {
    Available,
    Completed,
    Locked,
};

class SaveGame {
    public:
        SaveGame();
        void completed(unsigned int level_no);
        LevelState level_state(unsigned int level_no);
        /* World unlocked (0-4) */
        unsigned int world();
        unsigned int next_level();
        virtual void load();
    protected:
        const char *savedir;
        unsigned char completed_levels[LEVELS_BYTES];
        /* Called on anything which changes state */
        virtual void save();
};

#ifdef LINUX
#include "save.linux.h"
#endif

#endif
