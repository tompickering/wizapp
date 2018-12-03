#include "character.h"

#include <string>

#include "block.h"
#include "breakable.h"
#include "collectable.h"

#include "anim/animation.h"
#include "anim/animation_boomerang.h"

#include "shared.h"

using std::string;

Character::Character(int x, int y, CharacterType ctype) : Entity(x, y) {
    this->type = ctype;
    this->name = "Character";
    this->sort_key = 1;
    this->force_move_pending = false;
    this->force_move_left = false;
    this->force_move_breakable = NULL;
    if (ctype == Wizard) {
        this->b = 0xFF;
    } else {
        this->g = this->b = 0xFF;
    }
    this->pending_collectable = nullptr;
    this->move_just_completed = false;
    this->x_draw_offset = -0.4;
    this->y_draw_offset = -0.1;

    this->state = Idling;
    this->facing = FacingRight;

    this->turn_rate = 0.1f;

    this->anim = NULL;
    this->anim_base = ctype == Wizard ? "assets/img/wiz/" : "assets/img/blob/";

    this->move_speed_x = 1.f / WALK_DURATION;
    this->move_speed_y = 1.f / CLIMB_DURATION;

    this->walk_phase = 0;
}

Character::~Character() {
    if (this->anim) {
        delete this->anim;
        this->anim = NULL;
    }
}

void Character::single_block_move_complete() {
    Entity::single_block_move_complete();
    this->move_just_completed = true;
}

bool Character::receptive_to_input() {
    return true;
}

void Character::update(float delta_time) {
    level_ref->update_falling();
    Entity::update(delta_time);

    this->update_anim(delta_time);

    if (this->pending_collectable && this->real_x == this->pending_collectable->real_x) {
        this->pending_collectable->ignore = true;
    }

    if (this->move_just_completed) {
        /* Wait one frame for other things to catch up (falling blocks etc) */
        this->move_just_completed = false;
        return;
    }

    if (level_ref->active_character && level_ref->active_character->type != this->type)
        return;

    if (!this->at_rest() || !level_ref->stable())
        return;

    if (this->state != Idling)
        return;

    if (this->force_move_pending) {
        if (this->force_move_breakable->navigable_h) {
            logger.debug("Executing forced move");
            this->force_move_breakable->ignore = true;
            this->move(force_move_breakable->block_x, force_move_breakable->block_y);
            this->force_move_pending = false;
            this->force_move_breakable = NULL;
        }
        /* Don't try to make another move now */
        return;
    }

    bool up = input_manager.read(Up, false);
    bool down = input_manager.read(Down, false);
    bool left = input_manager.read(Left, false);
    bool right = input_manager.read(Right, false);
    bool morph = input_manager.read(Space, true);

    bool move_attempted = false;
    bool attempting_climb = false;
    int input_target_x = this->block_x;
    int input_target_y = this->block_y;
    if (left ^ right) {
        move_attempted = true;
        if (left) {
            input_target_x--;
        } else {
            input_target_x++;
        }
    } else if (up ^ down) {
        move_attempted = true;
        attempting_climb = true;
        if (up) {
            input_target_y--;
        } else {
            input_target_y++;
        }
    }

    if (move_attempted) {
        /* Handle turning */
        if ((this->facing == FacingRight && input_target_x < this->block_x)
            || (this->facing == FacingLeft && input_target_x > this->block_x)) {
            this->state = Turning;
            return;
        }

        if (level_ref->is_navigable(input_target_x, input_target_y,
                                    this->block_x, this->block_y,
                                    attempting_climb)) {
            this->move(input_target_x, input_target_y);
        } else if (left ^ right) {
            /* Interactions with block and breakable */
            Entity* target = level_ref->at(input_target_x, input_target_y);
            Block* block = dynamic_cast<Block*>(target);
            Breakable* breakable = dynamic_cast<Breakable*>(target);
            if (block) {
                if (block->push(left)) {
                    logger.debug("Interact with block");
                    this->state = Pushing;
                    audio_manager.play_sfx(Wop);
                }
            } else if (breakable) {
                if (breakable->break_block()) {
                    logger.debug("Interact with breakable");
                    this->state = Breaking;
                    audio_manager.play_sfx(Break);
                    this->force_move_pending = true;
                    this->force_move_left = left;
                    this->force_move_breakable = breakable;
                }
            }
        }
        pending_collectable = level_ref->attempt_collect(this->block_x, this->block_y);
    } else if (morph) {
        if (level_ref->switch_character()) {
            audio_manager.play_sfx(Morph);
            this->state = Morphing;
            level_ref->active_character->state = Morphing;
        }
    }
}

/* All anim allocation and freeing handled here */
/* Do not allocate a new value without freeing the old one */
void Character::update_anim(float delta_time) {
    if (this->anim) {
        if (this->anim->complete) {
            delete this->anim;
            this->anim = NULL;

            if (this->state == Turning) {
                this->facing = (this->facing == FacingLeft ? FacingRight : FacingLeft);
            }

            if (this->walk_phase == 0 && this->state == Walking) {
                this->walk_phase++;
            } else {
                this->state = Idling;
                this->walk_phase = 0;
            }
        } else {
            this->anim->advance(delta_time);
        }
    }

    if (!this->anim) {
        /* Do we need to assign a new one? */
        string anim_base;

        if (this->state == Turning) {
            anim_base = this->anim_base + "turn";
            if (this->type == Wizard) {
                if (this->facing == FacingLeft) {
                    this->anim = new Animation(anim_base, 7, 0.12f, true);
                } else {
                    this->anim = new Animation(anim_base, 7, 0.12f, false);
                }
            } else {
                if (this->facing == FacingLeft) {
                    this->anim = new Animation(anim_base, 6, 0.12f, true);
                } else {
                    this->anim = new Animation(anim_base, 6, 0.12f, false);
                }
            }
        }

        if (this->state == ClimbingUp || this->state == ClimbingDown) {
            if (this->type == Wizard) {
                anim_base = this->anim_base + "climb";
                this->anim = new Animation(anim_base, 22, CLIMB_DURATION, this->state == ClimbingDown);
            } else {
                anim_base = this->anim_base
                          + (this->facing == FacingLeft ? "left" : "right")
                          + "_climb_"
                          + (this->state == ClimbingUp ? "up" : "down");
                this->anim = new AnimationBoomerang(anim_base, 3, CLIMB_DURATION / 3.f);
                ((AnimationBoomerang*)(this->anim))->set_iterations(2);
            }
        }

        if (this->state == Pushing) {
            anim_base = this->anim_base
                      + (this->facing == FacingLeft ? "left" : "right")
                      + "_push";
            this->anim = new Animation(anim_base, 12, PUSH_DURATION);
        }

        if (this->state == Breaking) {
            anim_base = this->anim_base
                      + (this->facing == FacingLeft ? "left" : "right")
                      + "_break";
            this->anim = new Animation(anim_base, 11, BREAK_DURATION);
        }

        if (this->state == Morphing) {
            if (this->type == Blob) {
                anim_base = this->anim_base
                          + (this->facing == FacingLeft ? "left" : "right")
                          + "_morph";
                this->anim = new Animation(anim_base, 14, MORPH_DURATION, level_ref->active_character != this);
            }
        }

        if (this->state == Walking) {
            anim_base = this->anim_base
                      + (this->facing == FacingLeft ? "left" : "right")
                      + (this->walk_phase == 0 ? "" : "_alt");
            this->anim = new AnimationBoomerang(anim_base, 5, WALK_DURATION / 4.f);
            ((AnimationBoomerang*)(this->anim))->set_iterations(1);
        }
    }

    if (!this->anim) {
        this->state = Idling;
    }
}

void Character::move(int x, int y) {
    if (x != this->block_x) {
        this->state = Walking;
    } else if (y > this->block_y) {
        this->state = ClimbingDown;
    } else if (y < this->block_y) {
        this->state = ClimbingUp;
    }
    Entity::move(x, y);
}

string Character::sprite() {
    if (this->anim)
        return this->anim->sprite();

    if (this->type == Wizard) {
        if (this->facing == FacingRight)
            return "assets/img/wiz/right.png";
        if (this->facing == FacingLeft)
            return "assets/img/wiz/left.png";
    }
    if (this->type == Blob) {
        if (this == level_ref->active_character || this->state == Morphing) {
            if (this->facing == FacingRight)
                return "assets/img/blob/right.png";
            if (this->facing == FacingLeft)
                return "assets/img/blob/left.png";
        } else {
            if (this->facing == FacingRight)
                return "assets/img/blob/block.png";
            if (this->facing == FacingLeft)
                return "assets/img/blob/block2.png";
        }
    }

    return "assets/img/wiz/right.png";
}
