#ifdef WINDOWS

#include "save.windows.h"

#include <string>
#include <fstream>
#include <windows.h>

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

SaveGameWindows::SaveGameWindows() {
    savedir = getenv("USERPROFILE");
}

void SaveGameWindows::load() {
    ifstream in(string(savedir) + "/.wizapp_save", ios::in | ios::binary);
    in.read((char*) &data, sizeof(SaveData));
    SaveGame::load();
    in.close();
}

void SaveGameWindows::save() {
    SaveGame::save();
    ofstream out(string(savedir) + "/.wizapp_save", ios::out | ios::binary);
    out.write((char*) &data, sizeof(SaveData));
    out.close();
}

#endif
