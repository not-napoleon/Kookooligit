#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
#include <init.h>
#include <map.h>
#include <draw_map.h>

/*
 * Game logic here
 */

void cleanup(GameState *state) {
  free_game_state(state);
}

typedef enum {
  Error, MoveLeft, MoveRight, MoveUp, MoveDown, Quit
} CommandCode;

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

      state->map->matrix[state->at_location.x][state->at_location.y].contains_player = 0;
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
      state->map->matrix[state->at_location.x][state->at_location.y].contains_player = 1;
      break;
    case SDL_QUIT:
      printf("Got quit signal by magic\n");
      state->is_running = 0;
      break;
    default:
      printf("got some other event\n");
  }
}

void SDL_SurfaceInfo(char * name, SDL_Surface *thing)
{
  printf("Surface %s: w:%d h:%d bpp:%d\n", name, thing->w, thing->h, thing->format->BitsPerPixel);
}

int main(int argc, char *argv[]) {

  GameState *state = allocate_game_state();
  get_configuration(state->config);
  initilize(state);

  printf("tile size is %li\n", sizeof(Tile));
  SDL_Event event;
  char msg1[] = "Welcome to Kookoolegit!";
  add_message(state->messages, msg1, state->font);
  render_messages(&state->config->message_window, state->screen, state->messages);
  state->need_to_redraw_map = 1;
  printf("entering main loop\n");
  while (state->is_running) {
    while (SDL_PollEvent(&event)) {
      handle_events(state, &event);
    }
    if (state->need_to_redraw_messages == 1) {
      render_messages(&state->config->message_window, state->screen, state->messages);
      state->need_to_redraw_messages = 0;
    }
    if (state->need_to_redraw_map == 1) {
      Point top_left;
      Point bottom_right;
      get_visible_region(state->map, state->map_window_x_chars,
          state->map_window_y_chars, &top_left, &bottom_right);
      render_map_window(state->map, &top_left, &bottom_right, state->screen,
          &state->config->map_window, state->at_width, state->line_height);
      state->need_to_redraw_map = 0;
    }

  }

  cleanup(state);

  return 0;
}
