#ifndef DRAW_MAP_H_INCLUDED
#define DRAW_MAP_H_INCLUDED

#include <map.h>
#include <graphics_wrapper.h>

typedef struct MapGraphicsState {
  int line_height;
  int at_width;
  int map_window_x_chars;
  int map_window_y_chars;

  // Flags
  unsigned int draw_cursor :1;
} MapGraphicsState;


int init_map_graphics();
void set_draw_cursor(MapGraphicsState *mgs);
void clear_draw_cursor(MapGraphicsState *mgs);
int render_map_window(MapSection *map, MapGraphicsState *mgs, Rect *map_window,
    Point at_location, Point cursor_location);

#endif
