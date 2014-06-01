#include "creature.h"
#include "combat.h"
#include "messages.h"

void melee(struct Creature *attacker, struct Creature *defender) {
  add_combat_message("You flail ineffectually at it");
}
