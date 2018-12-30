#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include <string>
#include <vector>

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
    protected:
        vector<unsigned int> completed_levels;
        /* Called on construction */
        virtual void load();
        /* Called on anything which changes state */
        virtual void save();
};

#ifdef LINUX
#include "save.linux.h"
#endif

#endif
