#ifndef GUARD_SAVE_WINDOWS_H
#define GUARD_SAVE_WINDOWS_H

#include "save.h"

class SaveGameWindows : public SaveGame {
    public:
        SaveGameWindows();
        virtual void load() override;
    protected:
        virtual void save() override;
};

#endif
