#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include <string>
#include <vector>

#include <cstdint>

using std::vector;

enum LevelState {
    Available,
    Completed,
    Locked,
};

typedef struct {
    /* uint32 for each world */
    uint32_t completed_levels[5];
} SaveData;

class SaveGame {
    public:
        SaveGame();
        void completed(unsigned int level_no);
        LevelState level_state(unsigned int level_no);
        /* World unlocked (0-4) */
        unsigned int world();
        unsigned int world(unsigned int);
        unsigned int next_level();
        virtual void load();
    protected:
        const char *savedir;
        SaveData data;
        /* Called on anything which changes state */
        virtual void save();
};

#ifdef LINUX
#include "save.linux.h"
#endif

#ifdef WINDOWS
#include "save.windows.h"
#endif

#endif
