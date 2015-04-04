#ifndef CREATURE_TYPE_H_INCLUDED
#define CREATURE_TYPE_H_INCLUDED

#include "lib/uthash/src/uthash.h"

#include "sprite.h"

struct CreatureType {
  enum sprite_ids sprite_id;
  const char *creature_type_id;  /* Unique id for this creature type */
  const char *description;
  const char *name;


  UT_hash_handle hh;
};

void init_creature_types();
struct CreatureType *get_creature_type_by_id(const char* id);

#endif
