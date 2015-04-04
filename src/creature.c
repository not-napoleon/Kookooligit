#include <stdlib.h>
#include <string.h>

#include "creature.h"

#define LOG_LEVEL LOG_DEBUG
#include "log.h"

static struct Creature *creatures = NULL;
static unsigned int next_id = 0;

int spawn(const char *creature_type_id) {
  const struct CreatureType *type;
  type = get_creature_type_by_id(creature_type_id);
  if (type == NULL) {
    ERROR("NULL creature type for id %s, perhaps you forgot to initalize types?\n", creature_type_id);
    exit(-1);
  }
  struct Creature *new_creature;
  new_creature = (struct Creature *)malloc(sizeof(struct Creature));
  new_creature->creature_id = ++next_id;
  new_creature->type = type;
  new_creature->ticks = 0;
  new_creature->is_alive = 1;
  DEBUG("Spawning new creature with id %d\n", new_creature->creature_id);

  HASH_ADD_INT(creatures, creature_id, new_creature);
  return new_creature->creature_id;
}

struct Creature *get_creature_by_id(int creature_id){
  struct Creature *target;
  HASH_FIND_INT(creatures, &creature_id, target);
  return target;
}

int creature_count() {
  return HASH_COUNT(creatures);
}

void free_creature_by_id(int creature_id) {
  struct Creature *target = get_creature_by_id(creature_id);
  if (target == NULL) {
    return;
  }
  HASH_DEL(creatures, target);
  free(target);
}

void free_creatures() {
  struct Creature *item, *tmp;
  HASH_ITER(hh, creatures, item, tmp) {
    HASH_DEL(creatures, item);
    free(item);
  }
}
