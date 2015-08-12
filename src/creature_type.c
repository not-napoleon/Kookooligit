#include <stdlib.h>
#include <string.h>

#include "creature_type.h"

#define LOG_LEVEL LOG_INFO
#include "log.h"

static struct CreatureType *creature_list;

int register_creature_type(const char *id, const char *name, const char *desc,
                           enum sprite_ids sprite_id) {
    struct CreatureType *new_type;

    HASH_FIND(hh, creature_list, id, strlen(id), new_type);
    if (new_type == NULL) {
        DEBUG("registering new creature type with id %s\n", id);
        new_type = (struct CreatureType *)malloc(sizeof(struct CreatureType));
        new_type->creature_type_id = strdup(id);
        new_type->description = strdup(desc);
        new_type->name = strdup(name);
        new_type->sprite_id = sprite_id;

        HASH_ADD_KEYPTR(hh, creature_list, id, strlen(id), new_type);
    } else {
        ERROR("Attempted to register duplicate creature type %s\n", id);
        return -1;
    }
    return 0;
}

struct CreatureType *get_creature_type_by_id(const char *id) {
    DEBUG("Getting creature type by ID %s\n", id);
    struct CreatureType *type;
    HASH_FIND(hh, creature_list, id, strlen(id), type);
    return type;
}

void init_creature_types() {
    /* Load some hard coded creature types */
    register_creature_type("HUMAN_0_00", "Human", "A race of improbable "
                                                  "ape-like creatures fond of "
                                                  "exploration and explosions",
                           Player_sprite);
    register_creature_type(
        "DAMAGED_BATTLE_BOT_0_00", "Damaged Battle Bot",
        "An alien battle droid, damaged in a previous battle and left behind",
        BattleBot_sprite);
}
