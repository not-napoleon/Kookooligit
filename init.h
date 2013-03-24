#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
#include <map.h>

typedef struct GameConfiguration {
  char *font_path;
  int point_size;
  //Main window
  int window_w;
  int window_h;

  //Layout Dimensions
  SDL_Rect message_window;
  SDL_Rect map_window;
  SDL_Rect status_window;
  SDL_Rect command_window;

} GameConfiguration;

typedef struct GameState {
  int is_running;
  GameConfiguration *config;
  SDL_Surface *screen;
  TTF_Font *font;
  MessageList *messages;
  int need_to_redraw_messages;
  int need_to_redraw_map;
  Point at_location;
} GameState;

int get_configuration(GameConfiguration *config);
void initilize(GameState *state);
void free_game_state(GameState *state);
GameState *allocate_game_state();

#endif
