#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
#include <init.h>
#include <map.h>
#include <draw_map.h>
#include <command.h>
#include <game.h>

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

  printf("tile size is %li\n", sizeof(Tile));
  SDL_Event event;
  char msg1[] = "Welcome to Kookoolegit!";
  add_message(state->messages, msg1, state->font);
  render_messages(&state->config->message_window, state->screen, state->messages);
  calculate_visible_tiles(state->map, state->at_location);
  state->need_to_redraw_map = 1;
  printf("entering main loop\n");
  while (state->is_running) {
    while (SDL_PollEvent(&event)) {
      CommandCode cmd;
      cmd = get_command(&event);
      process_command(state, cmd);
      printf("state is %d\n", state->state);
      printf("******************************\n");
    }
    if (state->need_to_redraw_messages == 1) {
      render_messages(&state->config->message_window, state->screen, state->messages);
      state->need_to_redraw_messages = 0;
    }
    if (state->need_to_redraw_map == 1) {
      Point top_left;
      Point bottom_right;
      get_visible_region(state->map, state->map_window_x_chars,
          state->map_window_y_chars, &top_left, &bottom_right);
      render_map_window(state->map, &top_left, &bottom_right, state->screen,
          &state->config->map_window, state->at_width, state->line_height,
          state->at_location, state->cursor_location);
      state->need_to_redraw_map = 0;
      printf("\n\n", state->state);
    }

  }

  cleanup(state);

  return 0;
}
