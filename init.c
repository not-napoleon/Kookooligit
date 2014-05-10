#include "SDL_ttf.h"

#include <SDL_Tools.h>

#include <command.h>
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

int default_key_mapping() {
  /*
   * In the absense of a config file, hard code the default key mappings
   */

  bind_command_name(MoveLeft, "h");
  bind_command_name(MoveLeft, "Keypad 4");
  bind_command_name(MoveDown, "j");
  bind_command_name(MoveDown, "Keypad 2");
  bind_command_name(MoveUp, "k");
  bind_command_name(MoveUp, "Keypad 8");
  bind_command_name(MoveRight, "l");
  bind_command_name(MoveRight, "Keypad 6");
  bind_command_name(MoveUpLeft, "y");
  bind_command_name(MoveUpLeft, "Keypad 7");
  bind_command_name(MoveDownLeft, "b");
  bind_command_name(MoveDownLeft, "Keypad 1");
  bind_command_name(MoveUpRight, "u");
  bind_command_name(MoveUpRight, "Keypad 9");
  bind_command_name(MoveDownRight, "n");
  bind_command_name(MoveDownRight, "Keypad 3");
  bind_command_name(Quit, "q");
  bind_command_name(EnterLookMode, ";");
  bind_command_name(ExitLookMode, "esc");
}

void initilize(GameState *state) {
  // init random number generator
  int seed_value;
  seed_value = seed();
  DEBUG("random number generator seeded with %d\n", seed);
  init_graphics(state->config->window_w, state->config->window_h);
  default_key_mapping();

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

