#include <stdio.h>

#include "SDL_ttf.h"

#include "SDL_Tools.h"

#include "command.h"
#include "command_list.h"
#include "creature_type.h"
#include "draw_map.h"
#include "graphics_wrapper.h"
#include "init.h"
#include "map.h"
#include "messages.h"
#include "player.h"
#include "random.h"
#include "sprite.h"
#include "tile.h"

#define LOG_LEVEL LOG_INFO
#include "log.h"

void load_commands_from_file(const char *filename) {

  /* TODO: As soon as we need to open files in more than one place, move this
   * to a wrapper library.
   */
  FILE *fp;
  /*
   * Microsoft has apparently decided fopen is insecure and wants people to
   * use their fopen_s tool.  The following is adapted from this Stack Overflow
   * answer to deal with that issue:
   * http://stackoverflow.com/a/14966637/529459
   */
#ifdef WIN32
  errno_t err;
  if( (err  = fopen_s( &fp, filename, "r" )) !=0 ) {
#else
  if ((fp = fopen(filename, "r")) == NULL) {
#endif
    CRITICAL("Cannot open config file %s!\n", filename);
    exit(-1);
  }

  char line[256];
  while (fgets(line, 255, fp)) {
    char arg1[256];
    char arg2[256];
    int point_size;
    if (line[0] == '#') {
      DEBUG("Skipping comment line");
    } else if (sscanf(line, "[BIND_COMMAND_NAME:%[^][:]:%[^][:]]", &arg1, &arg2)) {
      DEBUG("Read command binding %s => %s\n", arg1, arg2);
      bind_command_name(string_to_command_code(arg1), arg2);
    } else if (sscanf(line, "[SET_MAP_FONT:%[^][:]:%d]", &arg1, &point_size)) {
      DEBUG("Read map font path %s, size %d\n", arg1, point_size);
      init_sprites_from_font(arg1, point_size);
    } else if (sscanf(line, "[SET_MESSAGE_FONT:%[^][:]:%d]", &arg1, &point_size)) {
      DEBUG("Read message font path %s, size %d\n", arg1, point_size);
      set_message_font(arg1, point_size);
    } else if (sscanf(line, "[SET_STATUS_FONT:%[^][:]:%d]", &arg1, &point_size)) {
      DEBUG("Read status font path %s, size %d\n", arg1, point_size);
      set_status_font(arg1, point_size);
    } else if (sscanf(line, "[SET_COMMAND_FONT:%[^][:]:%d]", &arg1, &point_size)) {
      DEBUG("Read command font path %s, size %d\n", arg1, point_size);
      set_command_font(arg1, point_size);
    } else {
      ERROR("Unknown configuration line %s\n", line);
    }
  }
  fclose(fp);
}


int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
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
  bind_command_name(ExitLookMode, "Escape");
}

void initilize(GameState *state) {
  // init random number generator
  int seed_value;
  seed_value = seed();
  DEBUG("random number generator seeded with %d\n", seed);
  init_graphics(state->config->window_w, state->config->window_h);
  load_commands_from_file("config.txt");
  init_creature_types();
  init_player();

  state->need_to_redraw = 0;
  init_tile_types();
  clear_draw_cursor(state->map_graphics_state);
  generate_initial_map(state->map);

  state->is_running = 1;
  state->state = Move;

  TRACE("initilized\n");
}

