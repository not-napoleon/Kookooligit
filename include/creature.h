#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include "lib/uthash/src/uthash.h"

#include "creature_type.h"
#include "point.h"

struct Creature {
  unsigned int creature_id;
  const struct CreatureType *type;
  unsigned short int ticks;
  Point current_location;
  int is_onscreen :1;
  int is_alive :1;

  UT_hash_handle hh;
};

int spawn(const char *creature_type_id);
struct Creature *get_creature_by_id(int creature_id);
int creature_count();
void free_creature_by_id(int creature_id);
void free_creatures();

/* define NULL for creature hash lookpus */
static const int NO_CREATURE = 0;

#endif
