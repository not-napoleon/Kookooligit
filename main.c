#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
#include <init.h>

/*
 * Game logic here
 */

void cleanup(GameState *state) {
  free_game_state(state);
}

typedef enum {
  Error, MoveLeft, MoveRight, MoveUp, MoveDown
} CommandCode;

CommandCode parse_keypress(SDL_Event *event) {
  CommandCode ret_val;
  switch(event->key.keysym.sym) {
    case SDLK_h:
    case SDLK_KP4:
      ret_val = MoveLeft;
      break;
    case SDLK_j:
    case SDKL_KP2:
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
      cmd = parse_keypress(event);
      printf("The %i key was pressed!\n", cmd);
      sprintf(msg, "The %s key was pressed!",
        SDL_GetKeyName(event->key.keysym.sym));
      add_message(state->messages, msg, state->font);
      if (event->key.keysym.sym == 'q') {
        printf("Got quit signal from pressing q\n");
        state->is_running = 0;
      }
      state->need_to_redraw = 1;
      break;
    case SDL_QUIT:
      printf("Got quit signal by magic\n");
      state->is_running = 0;
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

  SDL_Event event;
  char msg1[] = "Welcome to Kookoolegit!";
  add_message(state->messages, msg1, state->font);
  render_messages(&state->config->message_window, state->screen, state->messages);
  printf("entering main loop\n");
  while (state->is_running) {
    while (SDL_PollEvent(&event)) {
      handle_events(state, &event);
    }
    if (state->need_to_redraw == 1) {
      render_messages(&state->config->message_window, state->screen, state->messages);
      state->need_to_redraw = 0;
    }

  }

  cleanup(state);

  return 0;
}
