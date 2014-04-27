#include <stdlib.h>

#include "SDL.h"

#include <graphics_wrapper.h>

#define LOGGING_ENABLED
#include <log.h>

static SDL_Window *screen == NULL;
static SDL_Renderer *main_renderer == NULL;

SDL_Window *get_screen() {
  if (screen == NULL) {
    CRITICAL("Screen not initilized!\n");
  }
  return screen;
}

SDL_Renderer *get_main_renderer() {
  if (main_renderer == NULL) {
    CRITICAL("Renderer not initilized!\n");
  }
  return main_renderer;
}

void init_graphics(int window_w, int window_h) {
  if (SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO) < 0) {
    ERROR( "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if (TTF_Init() < 0) {
    ERROR( "Unable to init SDL_ttf: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(TTF_Quit);

  screen = SDL_CreateWindow("Kookoolegit",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      window_w, window_h, 0);
  main_renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
}

void clear_rect(const Rect *r) {
  SDL_Rect tmp = {r->x, r->y, r->w, r->h};
  SDL_Surface *screen;
  screen = get_screen();
  int ret = SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));
  if (ret != 0) {
    CRITICAL("SDL_FillRect failed with exit code %d for rect "
       "(x:%d, y: %d, w:%d, h:%d)\n", ret, r->x, r->y, r->w, r->h);
    exit(-1);
  }
}

Rect make_rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {
  /*
   * helper function to build a rect out of its four component values
   */
  Rect ret;
  ret.x = x;
  ret.y = y;
  ret.w = w;
  ret.h = h;
  return ret;
}

void draw_rect(const Rect *r) {
  TRACE("Drawing rect x: %d, y: %d, w: %d, h: %d\n", r->x, r->y, r->w, r->h);
  SDL_Surface *screen;
  screen = get_screen();
  SDL_UpdateRect(screen, r->x, r->y, r->w, r->h);
  TRACE("Screen updated\n");
}
