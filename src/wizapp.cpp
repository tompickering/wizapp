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
Level *level_ref = nullptr;

unsigned int level_no;

#ifdef SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

void WizApp::init() {
#ifdef SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        logger.fatal("Could not initialise SDL");
    }
    if (TTF_Init() < 0) {
        logger.fatal("Could not initialise SDL_ttf");
    }
#endif
}

int WizApp::run() {
    this->init();
    signal(SIGINT, signal_handler);
    running = true;
    logger = Logger();
    logger.set_level(INFO);
    logger.info("Starting...");

    Intro intro;
    bool intro_playing = true;

    Event event;
    bool event_playing = false;

    level_no = 2;
    draw_manager.init();
    audio_manager.init();
    bool level_running = false;

    Level level = Level(Original, level_no);
    steady_clock::time_point clock_begin;
    steady_clock::time_point clock_end;
    steady_clock::duration time_span;
    float delta_time = 0.f;
    float update_time = 0.f;
    float next_level_pause = 0.f;
    while (running) {
        clock_begin = steady_clock::now();
        input_manager.update();

        if (intro_playing) {
            intro.update(delta_time);
            if (intro.complete)
                intro_playing = false;
        } else if (event_playing) {
            event.update(delta_time);
            if (event.complete) {
                event_playing = false;
                level_no = 21;
            }
        } else {

            if (!level_running) {
                if (level_no == 1)
                    this->play_scene(World1Start);
                else if (level_no == 16)
                    this->play_scene(World1Mid);
                else if (level_no == 21)
                    this->play_scene(World2Start);
                else if (level_no == 36)
                    this->play_scene(World2Mid);
                else if (level_no == 41)
                    this->play_scene(World3Start);
                else if (level_no == 56)
                    this->play_scene(World3Mid);
                else if (level_no == 61)
                    this->play_scene(World4Start);
                else if (level_no == 76)
                    this->play_scene(World4Mid);
                else if (level_no == 81)
                    this->play_scene(World5Start);
                else if (level_no == 96)
                    this->play_scene(World5Mid);

                if (level_no > 100) {
                    running = false;
                    break;
                } else {
                    level = Level(Original, level_no);
                    level_ref = &level;
                    level.load();
                    audio_manager.play_music(level.get_track(), false, true);
                    level_running = true;
                    next_level_pause = 0.f;
                }
            }

            if (input_manager.read(R, true) && !level.complete) {
                level.reset();
            }

            draw_manager.update(&level);
            if (next_level_pause == 0.f) {
                level.update(delta_time);
            }

            if (level.complete) {
                if (next_level_pause == 0.f) {
                    logger.info("Level complete!");
                    /* Fade out music when about to change */
                    if ((level.number % 10) == 0) {
                        audio_manager.fade_out(1000);
                    }
                }
                if (next_level_pause > 1.5f) {
                    level.clear();
                    level_running = false;

                    if (level_no == 20)
                        this->play_scene(World1End);
                    if (level_no == 40)
                        this->play_scene(World2End);
                    if (level_no == 60)
                        this->play_scene(World3End);
                    if (level_no == 80)
                        this->play_scene(World4End);
                    if (level_no == 100) {
                        this->play_scene(World5End);
                    }

                    if (level_no == 20) {
                        // Temporary feature toggle
                        event_playing = true;
                    } else {
                        level_no++;
                    }
                } else {
                    next_level_pause += delta_time;
                }
            }
        }

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
    }
    level.clear();
    level_ref = NULL;
    return 0;
}

void WizApp::play_scene(SceneRef scene_ref) {
    Scene scene(scene_ref);
    audio_manager.play_music(scene.music_path(), true, true);
    steady_clock::time_point clock_begin;
    steady_clock::time_point clock_end;
    steady_clock::duration time_span;
    float delta_time = 0.f;
    float update_time = 0.f;
    bool finished = false;
    float fade_time = 0.f;
    while (running && !finished) {
        clock_begin = steady_clock::now();

        draw_manager.update(&scene);
        scene.update(delta_time);

        if (scene.complete) {
            if (fade_time == 0.f) {
                audio_manager.fade_out(3000);
            }
            fade_time += delta_time;
            if (fade_time > 3.5f)
                finished = true;
        }

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
    }
}
