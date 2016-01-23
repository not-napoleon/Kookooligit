#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include "creature_type.h"
#include "point.h"

struct Creature {
    const struct CreatureType *type;
    unsigned short int ticks;
    Point current_location;
    int is_onscreen : 1;
    int is_alive : 1;
};

struct Creature *spawn(const char *creature_type_id);
int creature_count();
void free_creature(struct Creature *target);

#endif
