#include <stdlib.h>
#include <string.h>

#include "lib/uthash/src/uthash.h"

#include "point.h"
#include "creature.h"

#define LOG_LEVEL LOG_DEBUG
#include "log.h"

struct Creature {
  unsigned int creature_id;
  const struct CreatureType *type;
  unsigned short int ticks;
  Point current_location;
  int is_onscreen :1;

  UT_hash_handle hh;
};

static struct Creature *creatures = NULL;
static unsigned int next_id = 0;

int spawn(const char *creature_type_id) {
  const struct CreatureType *type;
  type = get_creature_type_by_id(creature_type_id);
  if (type == NULL) {
    ERROR("NULL creature type for id %s, perhaps you forgot to initalize types?\n", creature_type_id);
    exit(-1);
  }
  DEBUG("Spawning new creature\n");
  struct Creature *new_creature;
  new_creature = (struct Creature *)malloc(sizeof(struct Creature));
  new_creature->creature_id = ++next_id;
  new_creature->type = type;
  new_creature->ticks = 0;
}

struct Creature *get_creature_by_id(int creature_id){
  struct Creature *target;
  HASH_FIND_INT(creatures, &creature_id, target);
  return target;
}

int creature_count() {
  return HASH_COUNT(creatures);
}
