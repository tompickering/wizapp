#ifndef GUARD_ENTITY_H
#define GUARD_ENTITY_H

#include <string>

using std::string;

class Entity {
    public:
        Entity(int x, int y);
        virtual ~Entity();
        virtual void update(float delta_time);
        string name;
        float real_x, real_y = 0.f;  /* Still in block co-ordinates */
        int block_x, block_y = 0;
        int prev_block_x, prev_block_y = 0;
        /* Can entity be navigated into horizontally by a character? (e.g. collectables) */
        bool navigable_h = false;
        /* Can entity be navigated into vertically by a character? (ladders) */
        bool navigable_v = false;
        friend bool operator< (const Entity &lhs, const Entity &rhs);
        friend bool cmp_entity_ptr(Entity *lhs, Entity *rhs);
        unsigned char r, g, b; // Debug colour
        bool moving;
        bool flying;
        bool falling;
        bool ignore;
        float move_speed_x;
        float move_speed_y;
        virtual void move(int x, int y);
        bool at_rest();
        int initial_x;
        int initial_y;
        virtual void reset();
        virtual string sprite();
        float x_draw_offset;
        float y_draw_offset;
    protected:
        int sort_key;
        float fall_speed;
        virtual void single_block_move_complete();
};

bool operator< (const Entity &lhs, const Entity &rhs);
bool cmp_entity_ptr (Entity *lhs, Entity *rhs);

#endif
