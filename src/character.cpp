#include "character.h"

#include <string>

#include "block.h"
#include "breakable.h"
#include "collectable.h"

#include "anim/animation.h"
#include "anim/animation_boomerang.h"
#include "anim/animation_sequence.h"

#include "shared.h"

using std::string;

Character::Character(int _x, int _y, CharacterType ctype) : Entity(_x, _y) {
    type = ctype;
    name = "Character";
    sort_key = 1;

    x_draw_offset = -0.4;
    y_draw_offset = -0.1;

    if (ctype == Wizard) {
        b = 0xFF;
        anim_base = "assets/img/wiz/";
    } else if (ctype == Blob) {
        g = b = 0xFF;
        anim_base = "assets/img/blob/";
    } else if (ctype == Baddie) {
        r = g = b = 0;
        anim_base = "assets/img/scene/baddie/";
    }

    move_speed_x = 1.f / WALK_DURATION;
    move_speed_y = 1.f / CLIMB_DURATION;

    reset();

    fall_tween = 0.f;
    fall_entry_seconds = fall_speed / 30.f;

    /* Animations */
    anim_turn = new Animation(anim_base + "turn", type == Wizard ? 7 : 6, 0.12f);


    AnimationBoomerang *part1, *part2;
    anim_walk_left = new AnimationSequence();
    part1 = new AnimationBoomerang(anim_base + "left", 5, WALK_DURATION / 4.f);
    part2 = new AnimationBoomerang(anim_base + "left" + "_alt", 5, WALK_DURATION / 4.f);
    part1->set_iterations(1);
    part2->set_iterations(1);
    anim_walk_left->add_animation(part1);
    anim_walk_left->add_animation(part2);

    anim_walk_right = new AnimationSequence();
    part1 = new AnimationBoomerang(anim_base + "right", 5, 1.f);
    part2 = new AnimationBoomerang(anim_base + "right" + "_alt", 5, 1.f);
    part1->set_iterations(1);
    part2->set_iterations(1);
    anim_walk_right->add_animation(part1);
    anim_walk_right->add_animation(part2);

    if (type == Wizard) {
        anim_fall_left = new Animation("assets/img/wiz/left_fall", 8, 1.f);
        anim_fall_right = new Animation("assets/img/wiz/right_fall", 8, 1.f);
    }
}

Character::~Character() {
    if (anim) {
        delete anim;
        anim = NULL;
    }
}

void Character::reset() {
    Entity::reset();
    state = Idling;
    pending_collectable = nullptr;

    force_move_pending = false;
    force_move_left = false;
    force_move_breakable = NULL;

    facing = FacingRight;

    facing_tween = 1.f;
    turn_time = 0.15;

    anim = NULL;
}

bool Character::receptive_to_input() {
    return true;
}

void Character::update(float delta_time) {
    Entity::update(delta_time);
    bool anything_falling = level_ref->update_falling();

    if (falling) {
        if (type == Wizard)
            state = Falling;
        fall_tween += delta_time * 1.f/fall_entry_seconds;
        fall_tween = clamp(fall_tween, 0.f, 1.f);
    } else {
        fall_tween -= delta_time * 1.f/fall_entry_seconds;
        fall_tween = clamp(fall_tween, 0.f, 1.f);
    }

    if (state == Falling && !falling && fall_tween == 0.f)
        state = Idling;

    if (type == Wizard && state == Morphing)
        state = Idling;

    if (state != ClimbingDown) {
        /* TODO: Remove this and actually enter fall at the appropriate point */
        climbing_to_fall = false;
    }

    update_anim(delta_time);

    if (pending_collectable && real_x == pending_collectable->real_x) {
        pending_collectable->ignore = true;
    }

    bool up = input_manager.read(Up, false);
    bool down = input_manager.read(Down, false);
    bool left = input_manager.read(Left, false);
    bool right = input_manager.read(Right, false);

    if (state == ClimbingUp || state == ClimbingDown) {
        if (left) {
            facing = FacingLeft;
            facing_tween = 0.f;
        }
        if (right) {
            facing = FacingRight;
            facing_tween = 1.f;
        }
    }

    if (anything_falling) {
        return;
    }

    if (level_ref->active_character && level_ref->active_character->type != type)
        return;

    if (!at_rest() || !level_ref->stable())
        return;

    if (state == Turning) {
        /* Have we finished turning? */
        if (facing == FacingLeft && facing_tween == 1.f) {
            facing = FacingRight;
            state = Idling;
        } else if (facing == FacingRight && facing_tween == 0.f) {
            facing = FacingLeft;
            state = Idling;
        } else {
            facing_tween = clamp(facing_tween
                                 + delta_time * (1.f / turn_time)
                                 * (facing == FacingLeft ? 1.f : -1.f),
                                 0.f, 1.f);
        }
        /* Can't do anything else now */
        return;
    }

    if (state == Walking) {
        if (real_x == (float) block_x) {
            state = Idling;
        }
    }

    if (state != Idling)
        return;

    if (force_move_pending) {
        if (force_move_breakable->navigable_h) {
            logger.debug("Executing forced move");
            force_move_breakable->ignore = true;
            move(force_move_breakable->block_x, force_move_breakable->block_y);
            force_move_pending = false;
            force_move_breakable = NULL;
        }
        /* Don't try to make another move now */
        return;
    }

    bool move_attempted = false;
    bool attempting_climb = false;
    int input_target_x = block_x;
    int input_target_y = block_y;
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

    bool morph = input_manager.read(Space, true);

    if (move_attempted) {
        /* Handle turning */
        if ((facing == FacingRight && input_target_x < block_x)
            || (facing == FacingLeft && input_target_x > block_x)) {
            state = Turning;
            return;
        }

        if (level_ref->is_navigable(input_target_x, input_target_y,
                                    block_x, block_y,
                                    attempting_climb)) {
            if (down && level_ref->is_empty(input_target_x, input_target_y)) {
                /* Climbing down into the air - we'll shift into fall early! */
                climbing_to_fall = true;
            }
            move(input_target_x, input_target_y);
        } else if (left ^ right) {
            /* Interactions with block and breakable */
            Entity* target = level_ref->at(input_target_x, input_target_y);
            Block* block = dynamic_cast<Block*>(target);
            Breakable* breakable = dynamic_cast<Breakable*>(target);
            if (block) {
                if (block->push(left)) {
                    logger.debug("Interact with block");
                    state = Pushing;
                    audio_manager.play_sfx(Wop);
                }
            } else if (breakable) {
                if (breakable->break_block()) {
                    logger.debug("Interact with breakable");
                    state = Breaking;
                    force_move_pending = true;
                    force_move_left = left;
                    force_move_breakable = breakable;
                }
            }
        }
        pending_collectable = level_ref->attempt_collect(block_x, block_y);
    } else if (morph) {
        if (level_ref->switch_character()) {
            audio_manager.play_sfx(Morph);
            state = Morphing;
            level_ref->active_character->state = Morphing;
            /* Other character needs their anim updated immediately
             * to save frame of default active idle until their morph
             * animation can be set up */
            level_ref->active_character->update_anim(0);
        }
    }

    update_anim(0);
}

/* All anim allocation and freeing handled here */
/* Do not allocate a new value without freeing the old one */
void Character::update_anim(float delta_time) {
    if (anim) {
        /* Only if this is a 'playing' rather than a 'sampled' animation... */
        if (state != Turning
            && state != Walking) {
            if (anim->complete) {
                delete anim;
                anim = NULL;

                state = Idling;
            } else {
                anim->advance(delta_time);
            }
        }
    }

    if (!anim) {
        /* Do we need to assign a new one? */
        string image_base;


        if (type == Baddie) {
            if (state == Walking) {
                image_base = anim_base
                          + (facing == FacingLeft ? "left" : "right");
                anim = new AnimationBoomerang(image_base, 7, WALK_DURATION / 2.f);
                ((AnimationBoomerang*)(anim))->set_iterations(1);
            }
        } else {
            if (state == ClimbingUp || state == ClimbingDown) {
                if (type == Wizard) {
                    image_base = anim_base + "climb";
                    anim = new Animation(image_base, 22, CLIMB_DURATION, state == ClimbingDown);
                } else {
                    image_base = anim_base
                              + (facing == FacingLeft ? "left" : "right")
                              + "_climb_"
                              + (state == ClimbingUp ? "up" : "down");
                    anim = new AnimationBoomerang(image_base, 3, CLIMB_DURATION / 3.f);
                    ((AnimationBoomerang*)(anim))->set_iterations(2);
                }
            }

            if (state == Pushing) {
                image_base = anim_base
                          + (facing == FacingLeft ? "left" : "right")
                          + "_push";
                anim = new Animation(image_base, 12, PUSH_DURATION);
            }

            if (state == Breaking) {
                image_base = anim_base
                          + (facing == FacingLeft ? "left" : "right")
                          + "_break";
                anim = new Animation(image_base, 11, BREAK_DURATION);
            }

            if (state == Morphing) {
                if (type == Blob) {
                    image_base = anim_base
                              + (facing == FacingLeft ? "left" : "right")
                              + "_morph";
                    anim = new Animation(image_base, 14, MORPH_DURATION, level_ref->active_character != this);
                }
            }
        }
    }
}

void Character::move(int _x, int _y) {
    if (_x != block_x) {
        state = Walking;
    } else if (_y > block_y) {
        state = ClimbingDown;
    } else if (_y < block_y) {
        state = ClimbingUp;
    }
    Entity::move(_x, _y);
}

string Character::sprite() {
    if (anim) {
        return anim->sprite();
    }

    if (state == Turning) {
        return anim_turn->sprite(1.f - facing_tween);
    } else if (state == Walking) {
        Animation *anim_walk = facing == FacingLeft ? anim_walk_left : anim_walk_right;
        float interp = 0.f;
        if (facing == FacingLeft) {
            interp = 1.f - (real_x - (float) block_x);
        } else {
            interp = 1.f - ((float) block_x - real_x);
        }
        return anim_walk->sprite(interp);
    } else if (type == Wizard && state == Falling) {
        Animation *anim_fall = facing == FacingLeft ? anim_fall_left : anim_fall_right;
        return anim_fall->sprite(fall_tween);
    }

    if (type == Wizard) {
        if (facing == FacingRight)
            return "assets/img/wiz/right.png";
        if (facing == FacingLeft)
            return "assets/img/wiz/left.png";
    }
    if (type == Blob) {
        if (this == level_ref->active_character || state == Morphing) {
            if (facing == FacingRight)
                return "assets/img/blob/right.png";
            if (facing == FacingLeft)
                return "assets/img/blob/left.png";
        } else {
            if (facing == FacingRight)
                return "assets/img/blob/block.png";
            if (facing == FacingLeft)
                return "assets/img/blob/block2.png";
        }
    }
    if (type == Baddie) {
            if (facing == FacingRight)
                return "assets/img/scene/baddie/right01.png";
            if (facing == FacingLeft)
                return "assets/img/scene/baddie/left01.png";
    }

    return "assets/img/wiz/right.png";
}
