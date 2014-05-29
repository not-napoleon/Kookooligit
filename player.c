#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#define MAX_STATUS_LEN 127

#define LOGGING_ENABLED
#include "log.h"
/*
 * There can only be one player, by definition.
 */

struct Player *player;

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
}

void free_player() {
  /*
   * Free the player object
   */
  if (player->status != NULL) {
    free(player->status);
  }
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
