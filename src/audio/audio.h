#ifndef GUARD_AUDIO_H
#define GUARD_AUDIO_H

#include <map>
#include <string>

using std::map;
using std::string;

enum SFX {
    Collect,
    Wop,
    Break,
    Awat,
    FrequencyClear,
    Morph
};

class AudioManager {
    public:
        virtual void init() = 0;
        /* Used for playing short SFX */
        virtual void play_music(string track, bool force_reset, bool looping) = 0;
        virtual void play_sfx(SFX) = 0;
        virtual void fade_out(int ms) = 0;
    protected:
        map<SFX, void*> sfx_data;
        string playing_track;
};

#ifdef SDL
#include "audio.SDL.h"
#endif

#endif
