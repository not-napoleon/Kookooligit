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
  //flags
  unsigned int is_running :1;
  unsigned int need_to_redraw_messages :1;
  unsigned int need_to_redraw_map :1;

  // Config
  GameConfiguration *config;

  // Graphics
  SDL_Surface *screen;
  TTF_Font *font;

  MessageList *messages;
  MapSection *map;
  Point at_location;
} GameState;

int get_configuration(GameConfiguration *config);
void initilize(GameState *state);
void free_game_state(GameState *state);
GameState *allocate_game_state();

#endif
