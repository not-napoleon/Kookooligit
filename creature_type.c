#include <stdlib.h>
#include <string.h>

#include <lib/uthash/src/uthash.h>

#include <creature_type.h>
#include <sprite.h>

struct CreatureType {
  enum sprite_ids sprite_id;
  const char *creature_type_id;  /* Unique id for this creature type */
  const char *description;
  const char *name;


  UT_hash_handle hh;
};

static struct CreatureType *creature_list;

int register_creature_type(const char* id, const char* name, const char* desc, enum sprite_ids sprite_id) {
  struct CreatureType *new_type;

  HASH_FIND(hh, creature_list, id, strlen(id), new_type);
  if (new_type == NULL) {
    new_type = (struct CreatureType*)malloc(sizeof(struct CreatureType));
    new_type->creature_type_id = id;
    new_type->description = desc;
    new_type->name = name;
    new_type->sprite_id = sprite_id;

    HASH_ADD(hh, creature_list, creature_type_id, strlen(id), new_type);
  } else {
    return -1;
  }
}

struct CreatureType *get_creature_type_by_id(const char *id) {
  struct CreatureType *type;
  HASH_FIND(hh, creature_list, id, strlen(id), type);
  return type;
}


void init_creature_types() {
  /* Load some hard coded creature types */
  register_creature_type("DAMAGED_BATTLE_BOT_0_00", "Damaged Battle Bot",
      "An alien battle droid, damaged in a previous battle and left behind", BattleBot_sprite);
}
