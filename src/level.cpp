#include "level.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "entity.h"
#include "block.h"
#include "breakable.h"
#include "character.h"
#include "collectable.h"
#include "ladder.h"
#include "static_block.h"

#include "shared.h"

using std::ifstream;
using std::string;

Level::Level(LevelSet set, unsigned int number) {
    char level_str[4];
    this->number = number;

    if (set == Original) {
        theme = static_cast<Theme>((number - 1) / 10);
    } else {
        this->theme = Valley;
    }

    sprintf(level_str, "%03d", number);
    logger.info("Loading level: " + string(level_str));
    logger.info("Theme: " + std::to_string(theme));
    this->file_path = "orig/" + string(level_str);
    this->complete = false;

    char theme_no_str[3];
    theme_no_str[2] = '\0';
    sprintf(theme_no_str, "%02d", (int)(this->theme) + 1);
    this->theme_base = "assets/img/themes/" + string(theme_no_str) + "/";
    logger.info(this->theme_base);
}

bool Level::switch_character() {
    if (this->blob) {
        if (this->active_character == this->wizard) {
            logger.info("Switching to blob");
            this->active_character = this->blob;
        } else {
            logger.info("Switching to wizard");
            this->active_character = this->wizard;
        }
        return true;
    }
    return false;
}

bool Level::update_falling() {
    Entity *ent;
    bool anything_falling = false;

    /* Nothing starts falling whilst something is still moving */
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->moving)
            return false;
    }

    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->flying)
            continue;
        ent->falling = false;
        if (!this->on_ladder(ent->block_x, ent->block_y)
            && this->is_empty(ent->block_x, ent->block_y + 1)) {
            ent->falling = true;
            anything_falling = true;
        }
    }

    return anything_falling;
}

void Level::load() {
    int collectables = 0;
    this->wizard = nullptr;
    this->blob = nullptr;
    logger.debug("Loading level " + this->file_path);
    ifstream file;
    file.open("levels/" + this->file_path);
    Entity *new_entity;
    Entity *new_entity2;
    char output[20];
    int row = 0;
    this->block_w = 0;
    this->block_h = 0;
    if (file.is_open()) {
        while (!file.eof()) {
            output[0] = '\0';
            file >> output;
            if (output[0] != '\0') {
                bool breakloop = false;
                int col = 0;
                for (char *c = output; !breakloop && *c != '\n' && *c != '\0'; ++c) {
                    new_entity = NULL;
                    new_entity2 = NULL;
                    switch (*c) {
                        case '.':   // Nothing!
                            break;
                        case 'W':   // Wizard + Ladder
                            new_entity2 = new Ladder(col, row);
                        case 'w':   // Wizard
                            if (this->wizard) {
                                logger.error("Multiple wizards!");
                            }
                            this->wizard = new Character(col, row, CharacterType::Wizard);
                            new_entity = this->wizard;
                            break;
                        case 'O':   // Blob + Ladder
                            new_entity2 = new Ladder(col, row);
                        case 'o':   // Blob
                            this->blob = new Character(col, row, CharacterType::Blob);
                            new_entity = this->blob;
                            break;
                        case 'l':   // Ladder
                            new_entity = new Ladder(col, row);
                            break;
                        case 'B':   // (Pushable) Block
                            new_entity = new Block(col, row);
                            break;
                        case 'b':   // (Breakable) Block
                            new_entity = new Breakable(col, row);
                            break;
                        case 'c':   // (Falling) Collectable
                            ++collectables;
                            new_entity = new Collectable(col, row, false);
                            break;
                        case 'C':   // (Flying) Collectable
                            ++collectables;
                            new_entity = new Collectable(col, row, true);
                            break;
                        case 'X':   // Static Block
                            new_entity = new StaticBlock(col, row);
                            break;
                        case '\0':
                        case '\n':
                            breakloop = true;
                        default:
                            logger.warn("Unknown level character: " + string(1, *c));
                            breakloop = true;
                    }
                    if (new_entity) {
                        this->entities.push_back(new_entity);
                        logger.debug("New " + new_entity->name + " at " +
                                     std::to_string(col) + "," + std::to_string(row));
                    }
                    if (new_entity2) {
                        this->entities.push_back(new_entity2);
                        logger.debug("New " + new_entity2->name + " at " +
                                     std::to_string(col) + "," + std::to_string(row));
                    }
                ++col;
                }
                if (this->block_w > 0 && this->block_w != col) {
                    logger.error("Inconsistent number of columns in level!");
                }
                this->block_w = col;
                row++;
            }
        }
    }
    file.close();
    if (!collectables) {
        logger.error("No collectables in level!");
    }
    this->block_h = row;
    logger.debug("Size: " + std::to_string(this->block_w) + "x" + std::to_string(this->block_h));

    /* Set up ladders */
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        Ladder *l = dynamic_cast<Ladder*>(this->entities[i]);
        if (l) {
            if (!on_ladder(l->block_x, l->block_y + 1)) {
                l->base_ladder = true;
            }
        }
    }

    // Ensure characters are at the end, and therefore drawn last (over ladders)
    std::sort(this->entities.begin(), this->entities.end(), cmp_entity_ptr);

    this->active_character = this->wizard;
}


void Level::update(float delta_time) {
    Entity *ent;

    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        this->entities[i]->update(delta_time);
    }

    /* Update falling status */
    this->update_falling();

    /* Check completion */
    this->complete = true;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        Collectable *c = dynamic_cast<Collectable*>(ent);
        if (c && !c->ignore) {
            this->complete = false;
            break;
        }
    }
}

void Level::clear() {
    logger.info("Clearing level");
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        delete this->entities[i];
    }
    this->entities.clear();
}

bool Level::on_ladder(int x, int y) {
    Entity *ent;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->block_x == x && ent->block_y == y) {
            if (dynamic_cast<Ladder*>(ent)) {
                return true;
            }
        }
    }
    return false;
}

bool Level::is_empty(int x, int y) {
    if (x < 0) return false;
    if (y < 0) return false;
    if (x >= this->block_w) return false;
    if (y >= this->block_h) return false;
    return (!this->at(x, y));
}

bool Level::is_navigable(int x, int y, int from_x, int from_y, bool updown) {
    Entity *ent;
    bool ladder = false; 
    if (x < 0 || y < 0 || x >= this->block_w || y >= this->block_h)
        return false;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->ignore) {
            continue;
        }
        if (ent->falling) {
            // Should only be falling when nothing else is moving anyway!
            continue;
        }
        if (ent->block_x == x && ent->block_y == y) {
            if (updown && !ent->navigable_v) {
                return false;
            }
            if (!updown && !ent->navigable_h) {
                return false;
            }
            if (dynamic_cast<Ladder*>(ent)) {
                ladder = true;
            }
        }
    }
    return !updown                              // Walking sideways
           || (ladder && from_y < y)            // Moving down into a ladder (would return sooner if blocked)
           || this->on_ladder(from_x, from_y);  // On a ladder (would return sooner if destination blocked)
}

Collectable* Level::attempt_collect(int x, int y) {
    Entity *ent;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->block_x == x && ent->block_y == y) {
            if (ent->ignore)
                continue;
            Collectable *c = dynamic_cast<Collectable*>(ent);
            if (c) {
                c->collect();
                return c;
            } else {
                // Collectables never overlap anything else, so we won't find one here
                return nullptr;
            }
        }
    }
    return nullptr;
}

/* WARNING: In the case of a character on a ladder, this will return only one! */
/* Will not return entity with 'ignore' flag true */
Entity* Level::at(int x, int y) {
    Entity *ent = NULL;
    if (x < 0) return ent;
    if (y < 0) return ent;
    if (x >= this->block_w) return ent;
    if (y >= this->block_h) return ent;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->ignore)
            continue;
        if (ent->block_x == x && ent->block_y == y) {
            return ent;
        }
        if (ent->prev_block_x == x && ent->prev_block_y == y) {
            return ent;
        }
    }
    return NULL;
}

/* The level is 'stable' if nothing is falling and everything is at its target location */
bool Level::stable() {
    Entity *ent;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        if (ent->ignore)
            continue;
        if (!ent->at_rest()) {
            return false;
        }
    }
    return true;
}

void Level::reset() {
    logger.info("Resetting level");
    Entity *ent;
    for (unsigned int i = 0; i < this->entities.size(); ++i) {
        ent = this->entities[i];
        ent->reset();
    }

    this->active_character = this->wizard;
}

string Level::get_theme_base() {
    return this->theme_base;
}

string Level::get_background() {
    return this->theme_base + "back/" + (((this->number - 1) % 10 < 5) ? "1" : "2") + ".png";
}

string Level::get_track() {
    if (this->number <= 10)
        return "assets/audio/TRACK1.ogg";
    if (this->number <= 20)
        return "assets/audio/TRACK2.ogg";
    if (this->number <= 30)
        return "assets/audio/TRACK3.ogg";
    if (this->number <= 40)
        return "assets/audio/TRACK4.ogg";
    if (this->number <= 50)
        return "assets/audio/TRACK5.ogg";
    if (this->number <= 60)
        return "assets/audio/TRACK6.ogg";
    if (this->number <= 70)
        return "assets/audio/TRACK7.ogg";
    if (this->number <= 80)
        return "assets/audio/TRACK8.ogg";
    if (this->number <= 90)
        return "assets/audio/TRACK9.ogg";
    return "assets/audio/TRACK10.ogg";
}
