#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "SDL.h"

typedef enum {
  NoOp,
  MoveLeft, MoveRight, MoveUp, MoveDown,
  MoveUpLeft, MoveDownLeft, MoveUpRight, MoveDownRight,
  EnterLookMode, ExitLookMode,
  Quit
} CommandCode;

CommandCode get_command(SDL_Event *event);

#endif
