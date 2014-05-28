#include <stdlib.h>
#include <string.h>

#include <lib/uthash/src/uthash.h>

#include <creature.h>

struct Creature {
  unsigned int creature_id;
  const struct CreatureType *type;
  unsigned short int ticks;

  UT_hash_handle hh;
};

static struct Creature *creatures = NULL;
static unsigned int next_id = 0;

int spawn(const struct CreatureType *type) {
  struct Creature *new_creature;
  new_creature = (struct Creature *)malloc(sizeof(struct Creature));
  new_creature->creature_id = next_id++;
  new_creature->type = type;
  new_creature->ticks = 0;
}

struct Creature *get_creature_by_id(int creature_id){
  struct Creature *target;
  HASH_FIND_INT(creatures, &creature_id, target);
  return target;
}
