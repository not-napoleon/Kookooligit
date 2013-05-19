#include <stdlib.h>

#include "SDL.h"

#include <graphics_wrapper.h>

#include <log.h>
void clear_rect(const Rect *r) {
  SDL_Rect tmp = {r->x, r->y, r->w, r->h};
  SDL_Surface *screen = SDL_GetVideoSurface();
  if (screen == NULL) {
    CRITICAL("SDL_GetVideoSurface returned null!\n");
  }
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
