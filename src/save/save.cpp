#include "save.h"

#include <algorithm>
#include <vector>

#include "../shared.h"

using std::vector;

SaveGame::SaveGame() {
    for (int i = 0; i < 5; ++i) {
        data.completed_levels[i] = 0;
    }
}

void SaveGame::completed(unsigned int level_no) {
    data.completed_levels[world(level_no)] |= 1 << ((level_no - 1) % 20);
    this->save();
}

LevelState SaveGame::level_state(unsigned int level_no) {
    if (data.completed_levels[world(level_no)] & 1 << ((level_no - 1) % 20))
        return Completed;
    if (world(level_no) > world())
        return Locked;
    if ((level_no - 1) % 20 >= 15) {
        for (unsigned int i = 1; i < level_no; ++i) {
            if (level_state(i) != Completed)
                return Locked;
        }
    }
    return Available;
}

unsigned int SaveGame::next_level() {
    for (unsigned int i = 1; i <= 100; ++i) {
        if (level_state(i) == Available)
            return i;
    }
    return 100;
}

unsigned int SaveGame::world() {
    for (int i = 3; i >= 0; --i) {
        if (data.completed_levels[i] == 0b11111111111111111111)
            return i + 1;
    }

    return 0;
}

unsigned int SaveGame::world(unsigned int level_no) {
    return (level_no - 1) / 20;
}

void SaveGame::load() {
    logger.info("SAVEGAME: Loading");
}

void SaveGame::save() {
    logger.info("SAVEGAME: Saving");
}
