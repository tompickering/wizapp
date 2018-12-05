#ifndef GUARD_CHARACTER_H
#define GUARD_CHARACTER_H

#include "entity.h"
#include "breakable.h"
#include "collectable.h"

#include "anim/animation.h"

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
    protected:
        void single_block_move_complete() override;
        void update_anim(float delta_time);
        Animation *anim;
        string anim_base;
        int walk_phase;
    private:
        FaceDirection facing;
        CharacterState state;
        float turn_rate;
        bool force_move_pending;
        bool force_move_left;
        bool move_just_completed;
        Breakable *force_move_breakable;
        Collectable *pending_collectable;
};

#endif
