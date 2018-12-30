#ifndef GUARD_SHARED_H
#define GUARD_SHARED_H

#define PROG_NAME "Wizard Apprentice"

#include "logger.h"
#include "level.h"
#include "audio/audio.h"
#include "input/input.h"
#include "draw/draw.h"
#include "save/save.h"

/* Everyone gets utils */
#include "utils.h"

#ifdef SDL
#define AUDIOMANAGER SDLAudioManager
#define DRAWMANAGER SDLDrawManager
#define INPUTMANAGER SDLInputManager
#endif

#ifdef LINUX
#define SAVEGAME SaveGameLinux
#endif

extern volatile bool running;
extern Logger logger;
extern AUDIOMANAGER audio_manager;
extern INPUTMANAGER input_manager;
extern DRAWMANAGER draw_manager;
extern Level *level_ref;

#endif
