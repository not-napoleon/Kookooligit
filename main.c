#include <stdlib.h>
#include "SDL.h"
#include "SDL_ttf.h"

typedef struct GameConfiguration {
  char *font_path;
} GameConfiguration;

typedef struct GameState {
  int is_running;
  GameConfiguration *config;
  SDL_Surface *screen;
  TTF_Font *font;
} GameState;

GameState *allocate_game_state() {
  /*Allocate a new game state struct
   */
  GameState *state;
  state = malloc(sizeof(GameState));
  state->config = malloc(sizeof(GameConfiguration));
  return state;
}

void free_game_state(GameState *state) {
  /* Recursively free GameState struct
   */
  free(state->config);
  free(state);
}

int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";

  return 0;
}

void initilize(GameState *state) {
  if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  // Get "best" video mode
  //const SDL_VideoInfo* best_settings = SDL_GetVideoInfo();
  state->screen = SDL_SetVideoMode(640, 480, 16, 0);
  if (state->screen == NULL) {
    fprintf(stderr, "Unable to set SDL video mode: %s\n", SDL_GetError());
    exit(1);
  }

  if (TTF_Init() < 0) {
    fprintf(stderr, "Unable to init SDL_ttf: %s\n", SDL_GetError());
    exit(1);
  }
  state->font = TTF_OpenFont(state->config->font_path, 16);
  if (state->font == NULL) {
    fprintf(stderr, "Unable to load font %s: %s\n", state->config->font_path,
        SDL_GetError());
    exit(1);
  }
  state->is_running = 1;
}

void cleanup(GameState *state) {
  free_game_state(state);
}

void handle_events(GameState *state, SDL_Event *event) {
  switch (event->type) {
    case SDL_KEYDOWN:
      printf("The %s key was pressed!\n",
        SDL_GetKeyName(event->key.keysym.sym));
      if (event->key.keysym.sym == 'q') {
        printf("Got quit signal from pressing q\n");
        state->is_running = 0;
      }
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

int DrawText(GameState *state, const char* text)
{
  SDL_Color color = {255,255,255};
  SDL_Surface *text_surface;

  printf("attempting to render text <<%s>>\n", text);


  SDL_SurfaceInfo("screen before", state->screen);
  text_surface = TTF_RenderText_Solid(state->font, text, color);
  SDL_SurfaceInfo("text_surface", text_surface);
  if (text_surface != NULL) {
    if (SDL_BlitSurface(text_surface, NULL, state->screen, NULL) < 0) {
      fprintf(stderr, "Blit failed with error: %s\n", SDL_GetError());
      exit(1);
    }
    SDL_FreeSurface(text_surface);
  } else {
    fprintf(stderr, "Unable to draw text: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_SurfaceInfo("screen after blit", state->screen);
  SDL_UpdateRect(state->screen, 0, 0, state->screen->w, state->screen->h);
  SDL_SurfaceInfo("screen after update", state->screen);
  return 1;
}

int main(int argc, char *argv[]) {

  GameState *state = allocate_game_state();
  get_configuration(state->config);
  initilize(state);

  SDL_Event *event;

  DrawText(state, "Hello World");
  while (state->is_running) {
    while (SDL_PollEvent(event)) {
      handle_events(state, event);
    }
  }

  cleanup(state);

  return 0;
}
