#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "creature.h"
#define MAX_STATUS_LEN 127

#define LOG_LEVEL LOG_ERROR
#include "log.h"
/*
 * There can only be one player, by definition.
 */

static int player_creature_id;
static struct Player *player;

void init_player() {
  /*
   * Allocate the player object
   */
  player = malloc(sizeof(struct Player));
  player->max_y = 0;
  player->curr_y = 0;
  player->turns = 0;
  player->status = malloc(sizeof(char) * MAX_STATUS_LEN);
  player->stale_status = 1;

  player_creature_id = spawn("HUMAN_0_00");
}

struct Creature *get_player_creature() {
  return get_creature_by_id(player_creature_id);
}

void free_player() {
  /*
   * Free the player object
   */
  if (player->status != NULL) {
    free(player->status);
  }
  /*
   * This is probably redundant, since if we're freeing the player,
   * we're probably freeing eveything else too, but why not be complete?
   */
  free_creature_by_id(player_creature_id);
  free(player);
}

void incr_distance(bool forward) {
  /*
   * Keep track of the player's progress through the corridor
   */
  player->curr_y += forward ? 1 : -1;
  if (abs(player->curr_y) > player->max_y) {
    player->max_y = abs(player->curr_y);
    player->stale_status = 1;
  }
}

void incr_turns() {
  /*
   * Turn counter
   */
  player->turns++;
}

char* get_player_status() {
  if (player->stale_status) {
    sprintf(player->status, "Max Y: %i", player->max_y);
    player->stale_status = 0;
  }
  return player->status;
}
