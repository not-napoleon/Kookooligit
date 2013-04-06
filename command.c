#include <command.h>

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
    case SDLK_n:
    case SDLK_KP1:
      ret_val = MoveDownLeft;
      break;
    case SDLK_u:
    case SDLK_KP9:
      ret_val = MoveUpRight;
      break;
    case SDLK_m:
    case SDLK_KP3:
      ret_val = MoveDownRight;
      break;
    case SDLK_q:
      ret_val = Quit;
      break;

    default:
      ret_val = Error;
    }
  return ret_val;
}

void handle_events(GameState *state, SDL_Event *event) {
  printf("Handling events\n");
  char msg[255];
  CommandCode cmd;
  switch (event->type) {
    case SDL_KEYDOWN:
      printf("got keypress event\n");
      cmd = parse_keypress(event);

      /*
      sprintf(msg, "The %s key was pressed!",
        SDL_GetKeyName(event->key.keysym.sym));
      add_message(state->messages, msg, state->font);
      state->need_to_redraw_messages = 1;
      */

      Point target_point = state->at_location;
      switch (cmd) {
        case MoveLeft:
          target_point.x -= 1;
          break;
        case MoveRight:
          target_point.x += 1;
          break;
        case MoveUp:
          target_point.y -= 1;
          break;
        case MoveDown:
          target_point.y += 1;
          break;
        case MoveUpLeft:
          target_point.x -= 1;
          target_point.y -= 1;
          break;
        case MoveDownLeft:
          target_point.x -= 1;
          target_point.y += 1;
          break;
        case MoveUpRight:
          target_point.x += 1;
          target_point.y -= 1;
          break;
        case MoveDownRight:
          target_point.x += 1;
          target_point.y += 1;
          break;
        case Quit:
          printf("Got quit signal from pressing q\n");
          state->is_running = 0;
          break;
      }
      if (is_passable_point(state->map, target_point) == 1) {
        state->at_location = target_point;
        state->need_to_redraw_map = 1;
      } else {
        char tmp[50] = "You can't walk through walls";
        add_message(state->messages, tmp, state->font);
        state->need_to_redraw_messages = 1;
      }
      // Decide if we need to recenter the map section
      if (abs(state->at_location.x - state->map->center.x) > 10) {
        state->map->center.x = state->at_location.x;
      }
      if (abs(state->at_location.y - state->map->center.y) > 10) {
        state->map->center.y = state->at_location.y;
      }
      printf("at_location (%i, %i), center (%i, %i)\n", state->at_location.x,
          state->at_location.y, state->map->center.x, state->map->center.y);
      calculate_visible_tiles(state->map, state->at_location);
      break;
    case SDL_QUIT:
      printf("Got quit signal by magic\n");
      state->is_running = 0;
      break;
    default:
      printf("got some other event\n");
  }
}

