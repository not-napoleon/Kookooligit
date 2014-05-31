#include "creature.h"
#include "combat.h"
#include "messages.h"

void melee(int creature_id) {
  struct Creature *enemy = get_creature_by_id(creature_id);
  add_combat_message("You flail ineffectually at it");
}
