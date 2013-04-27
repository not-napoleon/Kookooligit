#include <init.h>

#define LOGGING_ENABLED
#include <log.h>

SDL_Rect make_rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {
  /*
   * helper function to build a rect out of its four component values
   */
  SDL_Rect ret;
  ret.x = x;
  ret.y = y;
  ret.w = w;
  ret.h = h;
  return ret;
}

int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";
  config->point_size = 16;
  config->window_w = 1024;
  config->window_h = 768;

  // Windows are set as x, y, w, h
  config->map_window     = make_rect(  0,   0, 768, 640);
  config->command_window = make_rect(768,   0, 256, 640);
  config->message_window = make_rect(  0, 640, 768, 128);
  config->status_window  = make_rect(768, 640, 256, 128);

  return 0;
}

void initilize(GameState *state) {
  if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    ERROR( "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if (TTF_Init() < 0) {
    ERROR( "Unable to init SDL_ttf: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(TTF_Quit);

  state->screen = SDL_SetVideoMode(state->config->window_w, state->config->window_h, 16, 0);
  if (state->screen == NULL) {
    ERROR( "Unable to set SDL video mode: %s\n", SDL_GetError());
    exit(1);
  }

  // Load font
  state->font = TTF_OpenFont(state->config->font_path, state->config->point_size);
  state->map_graphics_state->font = TTF_OpenFont(state->config->font_path, state->config->point_size);

  if (state->font == NULL) {
    ERROR( "Unable to load font %s: %s\n", state->config->font_path,
        SDL_GetError());
    exit(1);
  }
  // Check for fixed with
  if (TTF_FontFaceIsFixedWidth(state->font) == 0) {
    ERROR( "Font is not fixed width, chances are nothing will look right");
  }
  // Turn off kerning, since we want our characters to line up in a grid
  if (TTF_GetFontKerning(state->font) != 0) {
    TTF_SetFontKerning(state->font, 0);
  }
  // set font size data
  int at_width, line_height;
  state->map_graphics_state->line_height = TTF_FontLineSkip(state->font);
  TTF_SizeText(state->font, "@", &state->map_graphics_state->at_width, NULL);
  state->map_graphics_state->map_window_x_chars = state->config->map_window.w / state->map_graphics_state->at_width;
  state->map_graphics_state->map_window_y_chars = state->config->map_window.h / state->map_graphics_state->line_height;
  INFO("map window size in characters is %i by %i\n", state->map_graphics_state->map_window_x_chars,
      state->map_graphics_state->map_window_y_chars);

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  state->need_to_redraw_messages = state->need_to_redraw_map = 0;
  init_map_graphics(state->map_graphics_state->font);
  generate_map(state->map);
  state->at_location.x = state->at_location.y = 10;
  state->map->center = state->at_location;

  state->is_running = 1;
  state->state = Move;

  TRACE("initilized\n");
}

