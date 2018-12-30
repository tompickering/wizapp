#ifndef GUARD_SAVE_LINUX_H
#define GUARD_SAVE_LINUX_H

#include "save.h"

class SaveGameLinux : public SaveGame {
    public:
        SaveGameLinux();
    protected:
        virtual void load() override;
        virtual void save() override;
};

#endif
