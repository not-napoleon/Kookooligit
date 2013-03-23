#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
typedef struct GameConfiguration {
  char *font_path;
  int point_size;
  //Main window
  int window_w;
  int window_h;

  //Message window
  int message_x;
  int message_y;
  int message_w;
  int message_h;

} GameConfiguration;

typedef struct GameState {
  int is_running;
  GameConfiguration *config;
  SDL_Surface *screen;
  TTF_Font *font;
  MessageList *messages;
  int need_to_redraw;
} GameState;

int get_configuration(GameConfiguration *config);
void initilize(GameState *state);
void free_game_state(GameState *state);
GameState *allocate_game_state();

#endif
