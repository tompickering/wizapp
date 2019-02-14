#include "entity.h"

#include "utils.h"
#include "shared.h"
#include "collectable.h"

Entity::Entity(int x, int y) {
    name = "Entity";
    sort_key = 0;
    r = g = b = 0;
    move_speed_x = 5.f;
    move_speed_y = 5.f;
    fall_speed = 10.f;
    flying = false;
    initial_x = x;
    initial_y = y;
    x_draw_offset = 0.f;
    y_draw_offset = 0.f;
    reset();
}

Entity::~Entity() {
}

void Entity::reset() {
    int x = initial_x;
    int y = initial_y;
    ignore = false;
    moving = false;
    falling = false;
    block_x = x;
    block_y = y;
    real_x = (float) x;
    real_y = (float) y;
    prev_block_x = x;
    prev_block_y = y;
}

/* N.B. 'Moving' relates only to voluntary motion - not falling! */
void Entity::move(int x, int y) {
    block_x = x;
    block_y = y;
    moving = true;
}

bool operator< (const Entity &lhs, const Entity &rhs) {
    return lhs.sort_key < rhs.sort_key;
}

bool cmp_entity_ptr(Entity *lhs, Entity *rhs) {
    return lhs->sort_key < rhs->sort_key;
}

void Entity::single_block_move_complete() {
    prev_block_x = block_x;
    prev_block_y = block_y;
    moving = false;
}

void Entity::update(float delta_time) {
    if (falling && real_y == (float) block_y) {
        block_y++;
    }
    float min_x, min_y = 0.f;
    float max_x, max_y = 10000.f;
    if (real_y != (float) block_y) {
        float sign = 1.;
        float speed = moving ? move_speed_y : fall_speed;
        if (real_y < (float) block_y) {
            sign = 1;
            min_y = 0.f;
            max_y = (float) block_y;
        } else {
            sign = -1.;
            min_y = (float) block_y;
            max_y = 10000.f;
        }
        real_y += sign * delta_time * speed;
        real_y = clamp(real_y, min_y, max_y);
        if (real_y == (float) block_y) {
            single_block_move_complete();
        }
    } else if (real_x != (float) block_x) {
        float sign = 1.;
        if (real_x < (float) block_x) {
            sign = 1.f;
            min_x = 0.f;
            max_x = (float) block_x;
        } else {
            sign = -1.;
            min_x = (float) block_x;
            max_x = 10000.f;
        }
        real_x += sign * delta_time * move_speed_x;
        real_x = clamp(real_x, min_x, max_x);
        if (real_x == (float) block_x) {
            single_block_move_complete();
        }
    }
}

bool Entity::at_rest() {
    return !falling
           && real_x == (float) block_x
           && real_y == (float) block_y;
}

string Entity::sprite() {
    return "";
}
