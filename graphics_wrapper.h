#ifndef GRAPHICS_WRAPPER_H_INCLUDED
#define GRAPHICS_WRAPPER_H_INCLUDED

#include <stdint.h>
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
void clear_rect(const Rect *r);
Rect make_rect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void draw_rect(const Rect *r);
void free_assets();

#endif
