#include "entity.h"

#include "utils.h"
#include "shared.h"
#include "collectable.h"

#define POS_THRESHOLD .0001

Entity::Entity(int x, int y) {
    this->name = "Entity";
    this->sort_key = 0;
    this->r = this->g = this->b = 0;
    this->move_speed_x = 5.f;
    this->move_speed_y = 5.f;
    this->fall_speed = 25.f;
    this->flying = false;
    this->initial_x = x;
    this->initial_y = y;
    this->x_draw_offset = 0.f;
    this->y_draw_offset = 0.f;
    this->reset();
}

Entity::~Entity() {
}

void Entity::reset() {
    int x = this->initial_x;
    int y = this->initial_y;
    this->ignore = false;
    this->moving = false;
    this->falling = false;
    this->block_x = x;
    this->block_y = y;
    this->real_x = (float) x;
    this->real_y = (float) y;
    this->prev_block_x = x;
    this->prev_block_y = y;
}

/* N.B. 'Moving' relates only to voluntary motion - not falling! */
void Entity::move(int x, int y) {
    this->block_x = x;
    this->block_y = y;
    this->moving = true;
}

bool operator< (const Entity &lhs, const Entity &rhs) {
    return lhs.sort_key < rhs.sort_key;
}

bool cmp_entity_ptr(Entity *lhs, Entity *rhs) {
    return lhs->sort_key < rhs->sort_key;
}

void Entity::single_block_move_complete() {
    this->prev_block_x = this->block_x;
    this->prev_block_y = this->block_y;
    this->moving = false;
}

void Entity::update(float delta_time) {
    if (this->falling && float_abs(this->real_y - (float) this->block_y) < POS_THRESHOLD) {
        this->block_y++;
    }
    float min_x, min_y = 0.f;
    float max_x, max_y = 10000.f;
    if (float_abs(this->real_x - (float) this->block_x) > POS_THRESHOLD) {
        float sign = 1.;
        if (this->real_x < (float) this->block_x) {
            sign = 1.f;
            min_x = 0.f;
            max_x = (float) this->block_x;
        } else {
            sign = -1.;
            min_x = (float) this->block_x;
            max_x = 10000.f;
        }
        this->real_x += sign * delta_time * this->move_speed_x;
        this->real_x = clamp(this->real_x, min_x, max_x);
        if (float_abs(this->real_x - (float) this->block_x) <= POS_THRESHOLD) {
            single_block_move_complete();
        }
    } else if (float_abs(this->real_y - (float) this->block_y) > POS_THRESHOLD) {
        float sign = 1.;
        float speed = this->moving ? this->move_speed_y : this->fall_speed;
        if (this->real_y < (float) this->block_y) {
            sign = 1;
            min_y = 0.f;
            max_y = (float) this->block_y;
        } else {
            sign = -1.;
            min_y = (float) this->block_y;
            max_y = 10000.f;
        }
        this->real_y += sign * delta_time * speed;
        this->real_y = clamp(this->real_y, min_y, max_y);
        if (float_abs(this->real_y - (float) this->block_y) <= POS_THRESHOLD) {
            single_block_move_complete();
        }
    }
}

bool Entity::at_rest() {
    return !this->falling
           && float_abs(this->real_x - (float) this->block_x) < POS_THRESHOLD
           && float_abs(this->real_y - (float) this->block_y) < POS_THRESHOLD;
}

string Entity::sprite() {
    return "";
}
