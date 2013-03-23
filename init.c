#include <init.h>

int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";
  config->point_size = 16;
  config->window_w = 1024;
  config->window_h = 768;

  config->message_window.x = 0;
  config->message_window.y = 640;
  config->message_window.w = config->window_w;
  config->message_window.h = config->window_h - config->message_window.y;

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
  TTF_CloseFont(state->font);
  free(state);
}
