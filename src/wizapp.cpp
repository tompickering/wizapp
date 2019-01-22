#include "wizapp.h"

#define MAX_FPS 60

#include "shared.h"
#include "audio/audio.h"
#include "draw/draw.h"
#include "input/input.h"
#include "level.h"
#include "logger.h"
#include "intro.h"
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
    state = GS_Intro;
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
}

void WizApp::update(float delta_time) {
    if (this->state == GS_Intro) {
        this->intro.update(delta_time);
        if (this->intro.complete)
            this->set_state(GS_StartLevel);
    } else if (this->state == GS_Event) {
        this->event.update(delta_time);
        if (this->event.complete) {
            this->set_state(GS_StartLevel);
            this->level_no = 21;
        }
    } else if (this->state == GS_StartLevScene || this->state == GS_EndLevScene) {
        if (this->scene) {
            draw_manager.update(this->scene);
            this->scene->update(delta_time);

            if (this->scene->complete) {
                if (this->fade_time == 0.f) {
                    audio_manager.fade_out(3000);
                }
                this->fade_time += delta_time;
                if (this->fade_time > 3.5f) {
                    this->fade_time = 0.f;
                    this->scene_just_played = this->scene->scene_ref;
                    delete this->scene;
                    this->scene = NULL;

                    if (this->state == GS_EndLevScene) {
                        if (this->level_no == 20) {
                            this->set_state(GS_Event);
                        } else {
                            this->level_no++;
                            this->set_state(GS_StartLevel);
                        }
                    }
                }
            }
        } else {
            this->set_state(GS_StartLevel);
            return;
        }
    } else if (this->state == GS_StartLevel) {
        bool scene_started = true;
        if (this->level_no == 1 && this->scene_just_played != World1Start) {
            this->scene = new Scene(World1Start);
        } else if (this->level_no == 16 && this->scene_just_played != World1Mid) {
            this->scene = new Scene(World1Mid);
        } else if (this->level_no == 21 && this->scene_just_played != World2Start) {
            this->scene = new Scene(World2Start);
        } else if (this->level_no == 36 && this->scene_just_played != World2Mid) {
            this->scene = new Scene(World2Mid);
        } else if (this->level_no == 41 && this->scene_just_played != World3Start) {
            this->scene = new Scene(World3Start);
        } else if (this->level_no == 56 && this->scene_just_played != World3Mid) {
            this->scene = new Scene(World3Mid);
        } else if (this->level_no == 61 && this->scene_just_played != World4Start) {
            this->scene = new Scene(World4Start);
        } else if (this->level_no == 76 && this->scene_just_played != World4Mid) {
            this->scene = new Scene(World4Mid);
        } else if (this->level_no == 81 && this->scene_just_played != World5Start) {
            this->scene = new Scene(World5Start);
        } else if (this->level_no == 96 && this->scene_just_played != World5Mid) {
            this->scene = new Scene(World5Mid);
        } else {
            scene_started = false;
        }

        if (scene_started) {
            this->set_state(GS_StartLevScene);
            return;
        }

        if (this->level_no > 100) {
            running = false;
            return;
        } else {
            this->level = new Level(Original, this->level_no);
            level_ref = this->level;
            this->level->load();
            audio_manager.play_music(level_ref->get_track(), false, true);
            this->next_level_pause = 0.f;
            this->set_state(GS_Level);
        }
    } else if (this->state == GS_EndLevel) {
        bool scene_started = true;
        if (this->level_no == 20) {
            this->scene = new Scene(World1End);
        } else if (this->level_no == 40) {
            this->scene = new Scene(World2End);
        } else if (this->level_no == 60) {
            this->scene = new Scene(World3End);
        } else if (this->level_no == 80) {
            this->scene = new Scene(World4End);
        } else if (this->level_no == 100) {
            this->scene = new Scene(World5End);
        } else {
            scene_started = false;
        }

        this->set_state(scene_started ? GS_EndLevScene : GS_StartLevel);

        if (this->state == GS_StartLevel)
            this->level_no++;

    } else if (this->state == GS_Level) {
        if (input_manager.read(R, true) && !level_ref->complete) {
            level_ref->reset();
        }

        draw_manager.update(level_ref);
        if (this->next_level_pause == 0.f) {
            level_ref->update(delta_time);
        }

        if (level_ref->complete) {
            if (this->next_level_pause == 0.f) {
                logger.info("Level complete!");
                savegame.completed(level_ref->number);
                /* Fade out music when about to change */
                if (((level_ref->number % 10) == 0) || ((level_ref->number + 5) % 20) == 0) {
                    audio_manager.fade_out(1000);
                }
            }
            if (this->next_level_pause > 1.5f) {
                level_ref->clear();
                delete this->level;
                this->level = NULL;
                this->set_state(GS_EndLevel);
            } else {
                this->next_level_pause += delta_time;
            }
        }
    }

    /* Ensure unused mouse click data is discarded */
    input_manager.read_click();
}

int WizApp::run() {
    this->init();
    running = true;

    steady_clock::time_point clock_begin;
    steady_clock::time_point clock_end;
    steady_clock::duration time_span;
    float delta_time = 0.f;
    float update_time = 0.f;

    while (running) {
        clock_begin = steady_clock::now();
        input_manager.update();
        this->speed = input_manager.read(LShift, false) ? 3.f : 1.f;
        this->update(delta_time);

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
        delta_time *= this->speed;
    }

    if (level_ref) {
        level_ref->clear();
        delete this->level;
        level_ref = NULL;
    }

    return 0;
}

void WizApp::set_state(GameState new_state) {
    logger.info(this->state2str(this->state) + " -> " + this->state2str(new_state));
    this->state = new_state;
}

string WizApp::state2str(GameState state) {
    switch (state) {
        case GS_Intro:
            return "Intro";
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
