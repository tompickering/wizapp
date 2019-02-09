#include "wizapp.h"

#define MAX_FPS 60

#include "shared.h"
#include "audio/audio.h"
#include "draw/draw.h"
#include "input/input.h"
#include "level.h"
#include "logger.h"
#include "intro.h"
#include "outro.h"
#include "event.h"

#include <csignal>
#include <cstdio>
#include <chrono>
#include <thread>

using std::chrono::steady_clock;

volatile bool running;

void signal_handler(int signum) {
    running = false;
}

Logger logger;
AUDIOMANAGER audio_manager;
INPUTMANAGER input_manager;
DRAWMANAGER draw_manager;
SAVEGAME savegame;
Level *level_ref = nullptr;

#ifdef SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

WizApp::WizApp() {
    scene = NULL;
    level = NULL;
    scene_just_played = None;
    level_no = 1;
    speed = 1.f;
    next_level_pause = 0.f;
    fade_time = 0.f;
}

void WizApp::init() {
    // Set up signal handler
    signal(SIGINT, signal_handler);

    logger = Logger();
    logger.set_level(INFO);
    logger.info("Starting...");

#ifdef SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        logger.fatal("Could not initialise SDL");
    }
    if (TTF_Init() < 0) {
        logger.fatal("Could not initialise SDL_ttf");
    }
#endif

    draw_manager.init();
    audio_manager.init();

    savegame.load();
    level_no = savegame.next_level();

    set_state(GS_Intro);
}

void WizApp::update(float delta_time) {
    if (state == GS_Intro) {
        intro.update(delta_time);
        if (intro.complete)
            set_state(GS_Menu);
    } else if (state == GS_Outro) {
        outro.update(delta_time);
        if (outro.complete)
            set_state(GS_Menu);
    } else if (state == GS_Menu) {
        menu.update(delta_time);
        if (menu.complete) {
            level_no = menu.get_level_to_start();
            menu.reset();
            set_state(GS_StartLevel);
        }
    } else if (state == GS_Event) {
        event.update(delta_time);
        if (event.complete) {
            set_state(GS_Menu);
        }
    } else if (state == GS_StartLevScene || state == GS_EndLevScene) {
        if (scene) {
            draw_manager.update(scene);
            scene->update(delta_time);

            if (scene->complete) {
                if (fade_time == 0.f) {
                    audio_manager.fade_out(3000);
                }
                fade_time += delta_time;
                if (fade_time > 3.5f) {
                    fade_time = 0.f;
                    scene_just_played = scene->scene_ref;
                    delete scene;
                    scene = NULL;

                    if (state == GS_EndLevScene) {
                        if (level_no == 20) {
                            set_state(GS_Event);
                        } else if (level_no == 100) {
                            set_state(GS_Outro);
                        } else {
                            set_state(GS_Menu);
                        }
                    }
                }
            }
        } else {
            set_state(GS_StartLevel);
            return;
        }
    } else if (state == GS_StartLevel) {
        bool scene_started = true;
        if (level_no == 1 && scene_just_played != World1Start) {
            scene = new Scene(World1Start);
        } else if (level_no == 16 && scene_just_played != World1Mid) {
            scene = new Scene(World1Mid);
        } else if (level_no == 21 && scene_just_played != World2Start) {
            scene = new Scene(World2Start);
        } else if (level_no == 36 && scene_just_played != World2Mid) {
            scene = new Scene(World2Mid);
        } else if (level_no == 41 && scene_just_played != World3Start) {
            scene = new Scene(World3Start);
        } else if (level_no == 56 && scene_just_played != World3Mid) {
            scene = new Scene(World3Mid);
        } else if (level_no == 61 && scene_just_played != World4Start) {
            scene = new Scene(World4Start);
        } else if (level_no == 76 && scene_just_played != World4Mid) {
            scene = new Scene(World4Mid);
        } else if (level_no == 81 && scene_just_played != World5Start) {
            scene = new Scene(World5Start);
        } else if (level_no == 96 && scene_just_played != World5Mid) {
            scene = new Scene(World5Mid);
        } else {
            scene_started = false;
        }

        if (scene_started) {
            set_state(GS_StartLevScene);
            return;
        }

        if (level_no > 100) {
            running = false;
            return;
        } else {
            level = new Level(Original, level_no);
            level_ref = level;
            level->load();
            audio_manager.play_music(level_ref->get_track(), false, true);
            next_level_pause = 0.f;
            set_state(GS_Level);
        }
    } else if (state == GS_EndLevel) {
        bool scene_started = true;
        if (level_no == 20) {
            scene = new Scene(World1End);
        } else if (level_no == 40) {
            scene = new Scene(World2End);
        } else if (level_no == 60) {
            scene = new Scene(World3End);
        } else if (level_no == 80) {
            scene = new Scene(World4End);
        } else if (level_no == 100) {
            scene = new Scene(World5End);
        } else {
            scene_started = false;
        }

        set_state(scene_started ? GS_EndLevScene : GS_StartLevel);

        if (state == GS_StartLevel)
            level_no = savegame.next_level();

    } else if (state == GS_Level) {
        if (input_manager.read(R, true) && !level_ref->complete) {
            level_ref->reset();
        }

        if (input_manager.read(Q, true) && !level_ref->complete) {
            set_state(GS_Menu);
            logger.debug("Quitting to menu");
            return;
        }

        draw_manager.update(level_ref);
        if (next_level_pause == 0.f) {
            level_ref->update(delta_time);
        }

        if (level_ref->complete) {
            if (next_level_pause == 0.f) {
                logger.debug("Level complete!");
                savegame.completed(level_ref->number);
                /* Fade out music when about to change */
                if (((level_ref->number % 10) == 0) || ((level_ref->number + 5) % 20) == 0) {
                    audio_manager.fade_out(1000);
                }
            }
            if (next_level_pause > 1.5f) {
                level_ref->clear();
                delete level;
                level = NULL;
                set_state(GS_EndLevel);
            } else {
                next_level_pause += delta_time;
            }
        }
    }

    /* Ensure unused mouse click data is discarded */
    input_manager.read_click();
}

int WizApp::run() {
    init();
    running = true;

    steady_clock::time_point clock_begin;
    steady_clock::time_point clock_end;
    steady_clock::duration time_span;
    float delta_time = 0.f;
    float update_time = 0.f;

    while (running) {
        clock_begin = steady_clock::now();
        input_manager.update();
        speed = input_manager.read(LShift, false) ? 3.f : 1.f;
        update(delta_time);

        /* Compute loop time and sleep */
        clock_end = steady_clock::now();
        time_span = clock_end - clock_begin;
        update_time = float(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
        float min_frame_time = 1.f / MAX_FPS;
        if (update_time < min_frame_time) {
            int sleep_ms = (int)(1000.f * min_frame_time - update_time);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }
        clock_end = steady_clock::now();
        time_span = clock_end - clock_begin;
        delta_time = float(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
        delta_time *= speed;
    }

    if (level_ref) {
        level_ref->clear();
        delete level;
        level_ref = NULL;
    }

    return 0;
}

void WizApp::set_state(GameState new_state) {
    logger.debug(state2str(state) + " -> " + state2str(new_state));
    state = new_state;
    switch (state) {
        case GS_Menu:
        case GS_RoundSelect:
            draw_manager.hide_mouse(false);
            break;
        default:
            draw_manager.hide_mouse(true);
            break;
    }
}

string WizApp::state2str(GameState state) {
    switch (state) {
        case GS_Intro:
            return "Intro";
        case GS_Outro:
            return "Outro";
        case GS_Menu:
            return "Menu";
        case GS_RoundSelect:
            return "RoundSelect";
        case GS_Event:
            return "Event";
        case GS_StartLevel:
            return "StartLevel";
        case GS_EndLevel:
            return "EndLevel";
        case GS_Level:
            return "Level";
        case GS_StartLevScene:
            return "StartLevScene";
        case GS_EndLevScene:
            return "EndLevScene";
        default:
            return "[UNKNOWN]";
    }
    return "[UNKNOWN]";
}
