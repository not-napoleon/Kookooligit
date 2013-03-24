#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"


typedef struct Point {
  /*
   * An x,y point on the map grid
   */
  int x;
  int y;
} Point;

int draw_map(Point at_location, TTF_Font *font, SDL_Surface *screen, const SDL_Rect *map_window);

#endif
