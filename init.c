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
    /*case SDLK_KP4:*/
      /*ret_val = MoveLeft;*/
      /*break;*/
    /*case SDLK_j:*/
    /*case SDLK_KP2:*/
      /*ret_val = MoveDown;*/
      /*break;*/
    /*case SDLK_k:*/
    /*case SDLK_KP8:*/
      /*ret_val = MoveUp;*/
      /*break;*/
    /*case SDLK_l:*/
    /*case SDLK_KP6:*/
      /*ret_val = MoveRight;*/
      /*break;*/
    /*case SDLK_y:*/
    /*case SDLK_KP7:*/
      /*ret_val = MoveUpLeft;*/
      /*break;*/
    /*case SDLK_b:*/
    /*case SDLK_KP1:*/
      /*ret_val = MoveDownLeft;*/
      /*break;*/
    /*case SDLK_u:*/
    /*case SDLK_KP9:*/
      /*ret_val = MoveUpRight;*/
      /*break;*/
    /*case SDLK_n:*/
    /*case SDLK_KP3:*/
      /*ret_val = MoveDownRight;*/
      /*break;*/
    /*case SDLK_q:*/
      /*ret_val = Quit;*/
      /*break;*/
    /*case SDLK_SEMICOLON:*/
      /*ret_val = EnterLookMode;*/
      /*break;*/
    /*case SDLK_ESCAPE:*/
      /*ret_val = ExitLookMode;*/
      /*break;*/
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

