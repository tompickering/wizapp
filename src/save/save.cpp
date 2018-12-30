#include "save.h"

#include <algorithm>
#include <vector>

#include "../shared.h"

using std::vector;

SaveGame::SaveGame() {
    this->load();
}

void SaveGame::completed(unsigned int level_no) {
    if (std::find(this->completed_levels.begin(),
                  this->completed_levels.end(),
                  level_no) == this->completed_levels.end()) {
        this->completed_levels.push_back(level_no);
    }
    std::sort(this->completed_levels.begin(),
              this->completed_levels.end());
    this->save();
}

LevelState SaveGame::level_state(unsigned int level_no) {
    if (level_no / 20 > this->world())
        return Locked;
    if (std::find(this->completed_levels.begin(),
                  this->completed_levels.end(),
                  level_no) != this->completed_levels.end())
        return Completed;
    return Available;
}

unsigned int SaveGame::world() {
    return this->completed_levels.size() / 20;
}

void SaveGame::load() {
    logger.info("SAVEGAME: Loading");
    std::sort(this->completed_levels.begin(),
              this->completed_levels.end());
}

void SaveGame::save() {
    logger.info("SAVEGAME: Saving");
}
