#include "SDL_ttf.h"

#include <SDL_Tools.h>

#include <draw_map.h>
#include <graphics_wrapper.h>
#include <init.h>
#include <map.h>
#include <messages.h>
#include <random.h>
#include <tile.h>
#include <sprite.h>
#define LOGGING_ENABLED
#include <log.h>


int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";
  config->point_size = 16;
  config->window_w = 1024;
  config->window_h = 768;

  // Windows are set as x, y, w, h
  config->map_window     = make_rect(  0,   0, 768, 640);
  config->command_window = make_rect(768,   0, 256, 640);
  config->message_window = make_rect(  0, 640, 768, 128);
  config->status_window  = make_rect(768, 640, 256, 128);

  return 0;
}

void initilize(GameState *state) {
  // init random number generator
  int seed_value;
  seed_value = seed();
  DEBUG("random number generator seeded with %d\n", seed);
  init_graphics(state->config->window_w, state->config->window_h);

  // Load font
  init_sprites_from_font(state->config->font_path, state->config->point_size);
  set_message_font(state->config->font_path, state->config->point_size);
  set_status_font(state->config->font_path, state->config->point_size);
  set_command_font(state->config->font_path, state->config->point_size);

  state->need_to_redraw = 0;
  init_tile_types();
  clear_draw_cursor(state->map_graphics_state);
  generate_initial_map(state->map);

  state->is_running = 1;
  state->state = Move;

  TRACE("initilized\n");
}

