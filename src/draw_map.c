#include <stdlib.h>

#include "draw_map.h"
#include "graphics_wrapper.h"
#include "point.h"
#include "sprite.h"

#define LOG_LEVEL LOG_ERROR
#include "log.h"

/*
 * File global to store the rendered map graphics
 */

void set_draw_cursor(MapGraphicsState *mgs) {
  TRACE("Setting draw cursor mode\n");
  mgs->draw_cursor = 1;
}

void clear_draw_cursor(MapGraphicsState *mgs) {
  TRACE("Clearing draw cursor mode\n");
  mgs->draw_cursor = 0;
}

int render_map_window(InfiniteMap *map, MapGraphicsState *mgs, Rect *map_window){

  int map_window_x_chars = get_char_width(map_window->w);
  int map_window_y_chars = get_char_height(map_window->h);
  DEBUG("attempting to render map - window size is %d x %d\n",
      map_window_x_chars, map_window_y_chars);

  struct Drawable **sprite_grid;
  /* Note to self: Why do I have dimensions + 1 tiles here? */
  sprite_grid = (struct Drawable **)malloc(sizeof(struct Drawable *)
      * (map_window_x_chars + 1));
  int i;
  for (i = 0; i <= map_window_x_chars; i++) {
    sprite_grid[i] = (struct Drawable *)malloc(sizeof(struct Drawable)
        * map_window_y_chars + 1);
  }
  get_tile_grid(map, map_window_x_chars, map_window_y_chars, mgs->draw_cursor, sprite_grid);

  DEBUG("Have tile grid, drawing sprites\n");
  int x, y;
  for (x = 0; x < map_window_x_chars; x++) {
    for (y = 0; y < map_window_y_chars; y++) {
      draw_sprite_at_point(&sprite_grid[x][y], x, y, map_window);
    }
  }

  for (i = 0; i < map_window_x_chars; i++) {
    free(sprite_grid[i]);
  }
  free(sprite_grid);
  return 1;
}
