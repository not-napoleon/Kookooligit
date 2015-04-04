#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "command.h"
#include "command_list.h"
#include "draw_map.h"
#include "graphics_wrapper.h"
#include "map.h"
#include "messages.h"

typedef enum {
  Move,
  Look
} States;

typedef struct GameConfiguration {
  //Main window
  int window_w;
  int window_h;

  //Layout Dimensions
  Rect message_window;
  Rect map_window;
  Rect status_window;
  Rect command_window;

} GameConfiguration;

typedef struct GameState {
  //flags
  unsigned int is_running :1;
  unsigned int need_to_redraw :1;

  // Config
  GameConfiguration *config;

  // Graphics

  // TODO: different message & map fonts
  // Map Data
  MapGraphicsState *map_graphics_state;

  char *status_message;
  InfiniteMap *map;

  States state;
} GameState;


void process_command(GameState *state, enum CommandCode cmd);
void free_game_state(GameState *state);
GameState *allocate_game_state();

#endif
