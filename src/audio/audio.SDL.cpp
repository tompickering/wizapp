#include "audio.SDL.h"

#include <SDL2/SDL_mixer.h>

#include "../shared.h"

SDLAudioManager::SDLAudioManager() {
}

void SDLAudioManager::init() {
    logger.info("AudioManager Init...");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logger.fatal("SDL_mixer failed to initialise");
    }
    playing_track = "";
    sfx_data[Collect] = (void*) Mix_LoadWAV("assets/sfx/collect.wav");
    sfx_data[Wop] = (void*) Mix_LoadWAV("assets/sfx/wop.wav");
    sfx_data[Break] = (void*) Mix_LoadWAV("assets/sfx/break.wav");
    sfx_data[Awat] = (void*) Mix_LoadWAV("assets/sfx/awat.wav");
    sfx_data[FrequencyClear] = (void*) Mix_LoadWAV("assets/sfx/frequencyclear.wav");
    sfx_data[Morph] = (void*) Mix_LoadWAV("assets/sfx/metamorph.wav");
}

void SDLAudioManager::play_music(string track, bool force_reset, bool looping) {
    logger.debug("Playing music");
    if (track == playing_track && !force_reset) {
        return;
    }
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    playing_track = track;
    Mix_Music *mus = Mix_LoadMUS(track.c_str());
    Mix_PlayMusic(mus, looping ? -1 : 1);
}

void SDLAudioManager::play_sfx(SFX sfx) {
    logger.debug("Playing SFX");
    Mix_PlayChannel(-1, (Mix_Chunk*) sfx_data[sfx], 0);
}

void SDLAudioManager::fade_out(int ms) {
    if (!Mix_PlayingMusic())
        return;
    Mix_FadeOutMusic(ms);
}
