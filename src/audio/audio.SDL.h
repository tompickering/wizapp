#ifndef GUARD_AUDIO_SDL_H
#define GUARD_AUDIO_SDL_H

#include "audio.h"

#include <string>

#include <SDL2/SDL_mixer.h>

using std::string;

class SDLAudioManager : public AudioManager {
    public:
        SDLAudioManager();
        virtual void init() override;
        virtual void play_music(string track, bool force_reset, bool looping) override;
        virtual void play_sfx(SFX) override;
        virtual void fade_out(int ms) override;
};

#endif
