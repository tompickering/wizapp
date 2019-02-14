#ifndef GUARD_CHARACTER_H
#define GUARD_CHARACTER_H

#include "entity.h"
#include "breakable.h"
#include "collectable.h"

#include "anim/animation.h"
#include "anim/animation_sequence.h"

#define WALK_DURATION 0.5f
#define CLIMB_DURATION 0.5f
#define PUSH_DURATION 0.25f
#define BREAK_DURATION 0.3f
#define MORPH_DURATION 0.4f

enum CharacterType {
    Wizard,
    Blob,
    Baddie
};

enum FaceDirection {
    FacingLeft,
    FacingRight
};

enum CharacterState {
    Idling,
    Turning,
    Walking,
    ClimbingUp,
    ClimbingDown,
    Pushing,
    Breaking,
    Falling,
    Landing,
    Morphing
};

class Character : public Entity {
    public:
        Character(int x, int y, CharacterType ctype);
        ~Character();
        CharacterType type;
        bool receptive_to_input();
        virtual string sprite() override;
        virtual void update(float delta_time) override;
        virtual void move(int x, int y) override;
        virtual void reset() override;

        /* Climbing down from a ladder into empty space */
        bool climbing_to_fall;

    protected:
        void update_anim(float delta_time);
        Animation *anim;
        string anim_base;
        float fall_tween;
        float fall_entry_seconds;
    private:
        FaceDirection facing;

        float facing_tween;
        float turn_time;

        CharacterState state;
        bool force_move_pending;
        bool force_move_left;
        Breakable *force_move_breakable;
        Collectable *pending_collectable;

        Animation *anim_turn;
        AnimationSequence *anim_walk_left;
        AnimationSequence *anim_walk_right;
        Animation *anim_fall_left;
        Animation *anim_fall_right;
};

#endif
