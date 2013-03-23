#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>

int DrawText( SDL_Surface *screen, TTF_Font *font, const char* text,
    int x, int y, int w, int h) {
  SDL_Color color = {255,255,255};
  SDL_Surface *text_surface;

  printf("attempting to render text <<%s>>\n", text);

  text_surface = TTF_RenderText_Solid(font, text, color);
  if (text_surface != NULL) {
    if (SDL_BlitSurface(text_surface, NULL, screen, NULL) < 0) {
      fprintf(stderr, "Blit failed with error: %s\n", SDL_GetError());
      exit(1);
    }
    SDL_FreeSurface(text_surface);
  } else {
    fprintf(stderr, "Unable to draw text: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
  return 1;
}

/*
 * Game logic here
 */


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

GameState *allocate_game_state() {
  /*Allocate a new game state struct
   */
  GameState *state;
  state = malloc(sizeof(GameState));
  state->config = malloc(sizeof(GameConfiguration));
  state->messages = init_message_list();
  return state;
}

void free_game_state(GameState *state) {
  /* Recursively free GameState struct
   */
  free(state->config);
  free_message_queue(state->messages);
  free(state);
}

int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";
  config->point_size = 16;
  config->window_w = 1024;
  config->window_h = 768;

  config->message_x = 0;
  config->message_y = 640;
  config->message_w = config->window_w;
  config->message_h = config->window_h - config->message_y;

  return 0;
}

void initilize(GameState *state) {
  if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if (TTF_Init() < 0) {
    fprintf(stderr, "Unable to init SDL_ttf: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(TTF_Quit);

  state->screen = SDL_SetVideoMode(state->config->window_w, state->config->window_h, 16, 0);
  if (state->screen == NULL) {
    fprintf(stderr, "Unable to set SDL video mode: %s\n", SDL_GetError());
    exit(1);
  }

  // Load font
  state->font = TTF_OpenFont(state->config->font_path, state->config->point_size);
  if (state->font == NULL) {
    fprintf(stderr, "Unable to load font %s: %s\n", state->config->font_path,
        SDL_GetError());
    exit(1);
  }
  // Check for fixed with
  if (TTF_FontFaceIsFixedWidth(state->font) == 0) {
    fprintf(stderr, "Font is not fixed width, chances are nothing will look right");
  }
  // Turn off kerning, since we want our characters to line up in a grid
  if (TTF_GetFontKerning(state->font) != 0) {
    TTF_SetFontKerning(state->font, 0);
  }

  state->is_running = 1;
  state->need_to_redraw = 0;
}

void cleanup(GameState *state) {
  free_game_state(state);
}

void handle_events(GameState *state, SDL_Event *event) {
  printf("Handling events\n");
  switch (event->type) {
    case SDL_KEYDOWN:
      printf("The %s key was pressed!\n",
        SDL_GetKeyName(event->key.keysym.sym));
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

  SDL_Event *event;

  //DrawText(state->screen, state->font, "Hello World", 0, 0, state->screen->w, state->screen->h);
  //char msg1[] = "first message";
  //char msg2[] = "Second Message";
  //char msg3[] = "Thrid Message";
  //add_message(state->messages, msg1, state->font);
  //add_message(state->messages, msg2, state->font);
  //add_message(state->messages, msg3, state->font);
  //render_messages(128, 128, 512, 512, state->screen, state->messages);
  printf("entering main loop\n");
  while (state->is_running) {
    while (SDL_PollEvent(event)) {
      handle_events(state, event);
    }
  }

  cleanup(state);

  return 0;
}
