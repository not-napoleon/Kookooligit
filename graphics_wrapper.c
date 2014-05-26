#include <stdlib.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <SDL_Tools.h>
#include <graphics_wrapper.h>

void init_graphics(int window_w, int window_h) {
  init_SDL(window_w, window_h);
}

void clear() {
  SDL_SetRenderDrawColor(get_main_renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(get_main_renderer());
}

void flip() {
  SDL_RenderPresent(get_main_renderer());
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
