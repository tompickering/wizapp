#include "save.linux.h"

#include <string>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#include "../shared.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

SaveGameLinux::SaveGameLinux() {
    savedir = getenv("HOME");

    if (!savedir) {
        savedir = getpwuid(getuid())->pw_dir;
    }

    logger.info("Using save directory " + string(savedir));
}

void SaveGameLinux::load() {
    ifstream in(string(savedir) + "/.wizapp_save", ios::in | ios::binary);
    in.read((char*) completed_levels, LEVELS_BYTES);
    SaveGame::load();
    in.close();
}

void SaveGameLinux::save() {
    SaveGame::save();
    ofstream out(string(savedir) + "/.wizapp_save", ios::out | ios::binary);
    out.write((char*) completed_levels, LEVELS_BYTES);
    out.close();
}
