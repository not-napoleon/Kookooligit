#include <SDL_Tools.h>

SDL_Color convert_color(Color c) {
  SDL_Color sc;
  sc.r = c.r;
  sc.g = c.g;
  sc.b = c.b;

  return sc;
}

