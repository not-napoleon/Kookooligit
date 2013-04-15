#ifndef DRAW_MAP_H_INCLUDED
#define DRAW_MAP_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

#include <map.h>

typedef struct MapGraphicsState {
  TTF_Font *font;
  int line_height;
  int at_width;
  int map_window_x_chars;
  int map_window_y_chars;

  // Flags
  unsigned int draw_cursor :1;
} MapGraphicsState;


int init_map_graphics(TTF_Font *font);
int render_map_window(MapSection *map, SDL_Surface *screen,
    MapGraphicsState *mgs, SDL_Rect *map_window,
    Point at_location, Point cursor_location);

#endif
