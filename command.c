#include <command.h>

//#define LOGGING_ENABLED
#include <log.h>

CommandCode parse_keypress(SDL_Event *event) {
  CommandCode ret_val;
  switch(event->key.keysym.sym) {
    case SDLK_h:
    case SDLK_KP4:
      ret_val = MoveLeft;
      break;
    case SDLK_j:
    case SDLK_KP2:
      ret_val = MoveDown;
      break;
    case SDLK_k:
    case SDLK_KP8:
      ret_val = MoveUp;
      break;
    case SDLK_l:
    case SDLK_KP6:
      ret_val = MoveRight;
      break;
    case SDLK_y:
    case SDLK_KP7:
      ret_val = MoveUpLeft;
      break;
    case SDLK_b:
    case SDLK_KP1:
      ret_val = MoveDownLeft;
      break;
    case SDLK_u:
    case SDLK_KP9:
      ret_val = MoveUpRight;
      break;
    case SDLK_n:
    case SDLK_KP3:
      ret_val = MoveDownRight;
      break;
    case SDLK_q:
      ret_val = Quit;
      break;
    case SDLK_SEMICOLON:
      ret_val = EnterLookMode;
      break;
    case SDLK_ESCAPE:
      ret_val = ExitLookMode;
      break;
    default:
      ret_val = NoOp;
    }
  return ret_val;
}


CommandCode get_command(SDL_Event *event) {
  /* Just call parse keypress if it's a keypress, and ignore it otherwise
   */
  CommandCode cmd;
  switch (event->type) {
    case SDL_KEYDOWN:
      TRACE("got keypress event\n");
      cmd = parse_keypress(event);
      break;
    case SDL_QUIT:
      TRACE("Got quit signal by magic\n");
      cmd = Quit;
      break;
    default:
      WARN("Unknown event type %d\n", event->type);
      cmd = NoOp;
      break;
  }
  return cmd;
}
