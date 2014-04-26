#include <stdlib.h>

#include "SDL.h"

#include <graphics_wrapper.h>

#define LOGGING_ENABLED
#include <log.h>

static SDL_Window *screen;

SDL_Window *get_screen() {
  if (screen == NULL) {
    CRITICAL("Screen not initilized!\n");
  }
  return screen;
}

void init_graphics(int window_w, int window_h) {
  screen = SDL_CreateWindow("Kookoolegit",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      window_w, window_h, 0);
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
