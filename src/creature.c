#include <stdlib.h>
#include <string.h>

#include "creature.h"

#define LOG_LEVEL LOG_DEBUG
#include "log.h"

struct Creature *spawn(const char *creature_type_id) {
    const struct CreatureType *type;
    type = get_creature_type_by_id(creature_type_id);
    if (type == NULL) {
        ERROR("NULL creature type for id %s, perhaps you forgot to initalize "
              "types?\n",
              creature_type_id);
        exit(-1);
    }
    struct Creature *new_creature;
    new_creature = (struct Creature *)malloc(sizeof(struct Creature));
    new_creature->type = type;
    new_creature->ticks = 0;
    new_creature->is_alive = 1;

    return new_creature;
}

void free_creature(struct Creature *target) {
    if (target == NULL) {
        return;
    }
    free(target);
}
