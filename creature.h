#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include <creature_type.h>

struct Creature;

int spawn(const struct CreatureType *type);
struct Creature *get_creature_by_id(int creature_id);
int creature_count();

/* define NULL for creature hash lookpus */
static const int NO_CREATURE = 0;

#endif
