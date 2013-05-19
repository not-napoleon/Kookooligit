#ifndef GRAPHICS_WRAPPER_H_INCLUDED
#define GRAPHICS_WRAPPER_H_INCLUDED

#include <inttypes.h>

typedef struct Rect {
  Sint16 x, y;
  Uint16 w, h;
} Rect;

void clear_rect(const Rect *r);
Rect make_rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h);

#endif
