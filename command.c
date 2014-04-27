#include "SDL.h"
#include <command.h>

//#define LOGGING_ENABLED
#include <log.h>

CommandCode parse_keypress(const SDL_Event event) {
  CommandCode ret_val;
  switch(event.key.keysym.sym) {
    case SDL_SCANCODE_H:
    case SDL_SCANCODE_KP_4:
      ret_val = MoveLeft;
      break;
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_KP_2:
      ret_val = MoveDown;
      break;
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_KP_8:
      ret_val = MoveUp;
      break;
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_KP_6:
      ret_val = MoveRight;
      break;
    case SDL_SCANCODE_Y:
    case SDL_SCANCODE_KP_7:
      ret_val = MoveUpLeft;
      break;
    case SDL_SCANCODE_B:
    case SDL_SCANCODE_KP_1:
      ret_val = MoveDownLeft;
      break;
    case SDL_SCANCODE_U:
    case SDL_SCANCODE_KP_9:
      ret_val = MoveUpRight;
      break;
    case SDL_SCANCODE_N:
    case SDL_SCANCODE_KP_3:
      ret_val = MoveDownRight;
      break;
    case SDL_SCANCODE_Q:
      ret_val = Quit;
      break;
    case SDL_SCANCODE_SEMICOLON:
      ret_val = EnterLookMode;
      break;
    case SDL_SCANCODE_ESCAPE:
      ret_val = ExitLookMode;
      break;
    default:
      ret_val = NoOp;
    }
  return ret_val;
}


CommandCode get_command() {
  /* Just call parse keypress if it's a keypress, and ignore it otherwise
   */
  SDL_Event event;
  SDL_WaitEvent(&event);
  CommandCode cmd;
  switch (event.type) {
    case SDL_KEYDOWN:
      TRACE("got keypress event\n");
      cmd = parse_keypress(event);
      break;
    case SDL_QUIT:
      TRACE("Got quit signal by magic\n");
      cmd = Quit;
      break;
    default:
      WARN("Unknown event type %d\n", event.type);
      cmd = NoOp;
      break;
  }
  return cmd;
}
