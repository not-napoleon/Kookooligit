#include <stdio.h>
#include <string.h>

#include "creature.h"
#include "combat.h"
#include "messages.h"
#include "random.h"

const char* miss_message = "You flail ineffectually at the %s.";
const char* hit_message = "You strike and kill the %s.";
const char* counter_attack_message = "The %s counterattacks and kills you.";


void melee(struct Creature *attacker, struct Creature *defender) {
  char *message = NULL;
  /*
   * This is going to over-allocate the string by the size of the format codes.
   * That seems like an acceptable cost for making this a relatively easy
   * calculation.
   */

  if (roll_die(2) == 1) {
    /*
     * Attacker hits defender
     */
    message = malloc(sizeof(char) * (strlen(hit_message)
          + strlen(defender->type->name)));
    sprintf(message, hit_message, defender->type->name);
    defender->is_alive = 0;
  } else {
    if (roll_die(10) == 1) {
      /*
       * Counter attack hit
       */
      message = malloc(sizeof(char) * (strlen(counter_attack_message)
            + strlen(defender->type->name)));
      sprintf(message, counter_attack_message, defender->type->name);
      attacker->is_alive = 0;
    } else {
      message = malloc(sizeof(char) * (strlen(miss_message)
            + strlen(defender->type->name)));
      sprintf(message, miss_message, defender->type->name);
    }
  }
  add_combat_message(message);
  free(message);
}
