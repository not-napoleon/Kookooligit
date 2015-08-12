#ifndef GRAPHICS_WRAPPER_H_INCLUDED
#define GRAPHICS_WRAPPER_H_INCLUDED

#include <stdint.h>

typedef struct Rect {
    int16_t x, y;
    uint16_t w, h;
} Rect;

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

void clear();
void init_graphics(int window_w, int window_h);
Rect make_rect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void flip();

#endif
