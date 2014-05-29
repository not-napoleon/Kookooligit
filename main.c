#include <stdlib.h>

#include "command.h"
#include "command_list.h"
#include "draw_map.h"
#include "game.h"
#include "graphics_wrapper.h"
#include "init.h"
#include "look.h"
#include "map.h"
#include "messages.h"
#include "player.h"
#include "render_text.h"

#define LOGGING_ENABLED
#include "log.h"

/*
 * Game logic here
 */

void cleanup(GameState *state) {
  free_game_state(state);
}


int main(int argc, char *argv[]) {

  GameState *state = allocate_game_state();
  get_configuration(state->config);
  initilize(state);

  INFO("tile size is %li\n", sizeof(Tile));
  char msg1[] = "Welcome to Kookoolegit! You are in an infinitely long "
    "non-orientable space.  Good luck!";
  add_message(state->messages, msg1);
  render_messages(&state->config->message_window, state->messages);
  calculate_visible_tiles(state->map, state->map->at_location);
  state->status_message = get_player_status();
  state->need_to_redraw = 1;
  TRACE("entering main loop\n");
  while (state->is_running) {
    /* This blocks, waiting for the next user input */
    DEBUG("******************************\n");
    DEBUG("Getting command\n");
    enum CommandCode cmd;
    cmd = get_command();
    process_command(state, cmd);
    DEBUG("state is %d\n", state->state);
    DEBUG("Redraw flag is %d\n", state->need_to_redraw);
    if (state->need_to_redraw == 1) {
      clear();
      render_messages(&state->config->message_window, state->messages);
      render_map_window(state->map, state->map_graphics_state,
          &state->config->map_window);
      render_look_message(state->status_message, &state->config->status_window);
      state->need_to_redraw = 0;
      flip();
    }
  }

  cleanup(state);

  return 0;
}
