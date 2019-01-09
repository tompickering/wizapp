#include "save.h"

#include <algorithm>
#include <vector>

#include "../shared.h"

using std::vector;

SaveGame::SaveGame() {
    for (int i = 0; i < LEVELS_BYTES; ++i) {
        completed_levels[i] = 0;
    }

    this->load();
}

void SaveGame::completed(unsigned int level_no) {
    this->completed_levels[(level_no - 1) / 8] &= 1 << ((level_no - 1) % 8);
    this->save();
}

LevelState SaveGame::level_state(unsigned int level_no) {
    if (level_no / 20 > this->world())
        return Locked;
    if (this->completed_levels[level_no / 8] & level_no % 8) {
        return Completed;
    }
    return Available;
}

unsigned int SaveGame::world() {
    for (int i = 3; i >= 0; --i) {
        if (   completed_levels[i * LEVELS_BYTES / 5] == 0xFF
            && completed_levels[(i * LEVELS_BYTES / 5) + 1] == 0xFF
            && completed_levels[(i * LEVELS_BYTES / 5) + 2] == 0xF0) {
        }
        return i + 1;
    }

    return 0;
}

void SaveGame::load() {
    logger.info("SAVEGAME: Loading");
}

void SaveGame::save() {
    logger.info("SAVEGAME: Saving");
}
