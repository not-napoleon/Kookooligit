#include <stdlib.h>

#include "SDL.h"

#include <messages.h>
#include <init.h>
#include <map.h>
#include <draw_map.h>
#include <render_text.h>
#include <command.h>
#include <game.h>

#define LOGGING_ENABLED
#include <log.h>

/*
 * Game logic here
 */

void cleanup(GameState *state) {
  free_game_state(state);
}


void SDL_SurfaceInfo(char * name, SDL_Surface *thing)
{
  printf("Surface %s: w:%d h:%d bpp:%d\n", name, thing->w, thing->h, thing->format->BitsPerPixel);
}

int main(int argc, char *argv[]) {

  GameState *state = allocate_game_state();
  get_configuration(state->config);
  initilize(state);

  INFO("tile size is %li\n", sizeof(Tile));
  SDL_Event event;
  char msg1[] = "Welcome to Kookoolegit!";
  add_message(state->messages, msg1, state->font);
  render_messages(&state->config->message_window, state->messages);
  calculate_visible_tiles(state->map, state->at_location);
  state->need_to_redraw_map = 1;
  TRACE("entering main loop\n");
  while (state->is_running) {
    while (SDL_PollEvent(&event)) {
      CommandCode cmd;
      cmd = get_command(&event);
      process_command(state, cmd);
      DEBUG("state is %d\n", state->state);
      DEBUG("******************************\n");
    }
    if (state->need_to_redraw_messages == 1) {
      render_messages(&state->config->message_window, state->messages);
      state->need_to_redraw_messages = 0;
    }
    if (state->need_to_redraw_map == 1) {
      render_map_window(state->map, state->map_graphics_state,
          &state->config->map_window, state->at_location,
          state->cursor_location);
      state->need_to_redraw_map = 0;
    }

  }

  cleanup(state);

  return 0;
}
