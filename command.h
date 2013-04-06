#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <game.h>
#include "SDL.h"

typedef enum {
  Error,
  MoveLeft, MoveRight, MoveUp, MoveDown,
  MoveUpLeft, MoveDownLeft, MoveUpRight, MoveDownRight,
  Quit
} CommandCode;

void handle_events(GameState *state, SDL_Event *event);

#endif
