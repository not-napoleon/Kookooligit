#ifndef GRAPHICS_WRAPPER_H_INCLUDED
#define GRAPHICS_WRAPPER_H_INCLUDED

#include <stdint.h>

/* TODO: Kill get_screen and get_main_renderer so I can get SDL out of this
 * header. */
#include "SDL.h"

typedef struct Rect {
  int16_t x, y;
  uint16_t w, h;
} Rect;

typedef struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color;

SDL_Renderer *get_main_renderer();
SDL_Window *get_screen();
void clear();
Rect make_rect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void flip();

#endif
