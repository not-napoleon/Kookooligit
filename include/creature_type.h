#ifndef CREATURE_TYPE_H_INCLUDED
#define CREATURE_TYPE_H_INCLUDED

#include "lib/uthash/src/uthash.h"

#include "sprite.h"

struct CreatureType {
  /*
   * Note about creature ids:
   * Creature ids have the form of NAME_MAJOR_MINOR, where major and minor are
   * version numbers.  This is to make it easier to manage cross-version
   * save file compatibility.
   */
  const char *creature_type_id;  /* Unique id for this creature type */
  const char *description;
  const char *name;
  enum sprite_ids sprite_id;

  /* TODO: Hide the hash in the implementation file - just add a wrapper
   * struct with a pointer to the actual creature type and the hash handle
   */
  UT_hash_handle hh;
};

void init_creature_types();
struct CreatureType *get_creature_type_by_id(const char* id);

#endif
