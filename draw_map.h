#ifndef DRAW_MAP_H_INCLUDED
#define DRAW_MAP_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

#include <map.h>

int init_map_graphics(TTF_Font *font);
int render_map_window(MapSection *map, Point *top_left, Point *bottom_right,
    SDL_Surface *screen, SDL_Rect *map_window, int at_width, int line_height,
    Point at_location, Point cursor_location);

#endif
