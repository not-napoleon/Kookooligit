#include "SDL_ttf.h"

#include <SDL_Tools.h>

#include <draw_map.h>
#include <graphics_wrapper.h>
#include <init.h>
#include <map.h>
#include <messages.h>
#include <random.h>
#include <tile.h>
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
  set_map_font(state->config->font_path, state->config->point_size);
  set_message_font(state->config->font_path, state->config->point_size);
  set_status_font(state->config->font_path, state->config->point_size);
  set_command_font(state->config->font_path, state->config->point_size);

  // Check for fixed with
  /* TODO: This should go in the font setup functions.  For example, some
   * kerning on message fonts could be nice.*/
  if (TTF_FontFaceIsFixedWidth(get_map_font()) == 0) {
    ERROR( "Font is not fixed width, chances are nothing will look right");
  }
  // Turn off kerning, since we want our characters to line up in a grid
  if (TTF_GetFontKerning(get_map_font()) != 0) {
    TTF_SetFontKerning(get_map_font(), 0);
  }
  // set font size data
  int at_width, line_height;
  state->map_graphics_state->line_height = TTF_FontLineSkip(get_map_font());
  TTF_SizeText(get_map_font(), "@", &state->map_graphics_state->at_width, NULL);
  /* TODO: Refactor this, probably into draw map, to deal with resizing */
  state->map_graphics_state->map_window_x_chars = state->config->map_window.w / state->map_graphics_state->at_width;
  state->map_graphics_state->map_window_y_chars = state->config->map_window.h / state->map_graphics_state->line_height;
  INFO("map window size in characters is %i by %i\n", state->map_graphics_state->map_window_x_chars,
      state->map_graphics_state->map_window_y_chars);

  state->need_to_redraw = 0;
  init_tile_types();
  clear_draw_cursor(state->map_graphics_state);
  generate_initial_map(state->map);

  state->is_running = 1;
  state->state = Move;

  TRACE("initilized\n");
}

