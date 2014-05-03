#include <draw_map.h>
#include <graphics_wrapper.h>
#include <point.h>
#include <sprite.h>

#define LOGGING_ENABLED
#include <log.h>

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

  DEBUG("attempting to render map - window size is %d x %d (line_height: %d, at_width: %d)\n",
      mgs->map_window_x_chars, mgs->map_window_y_chars, mgs->line_height, mgs->at_width);

  struct Drawable **tile_grid;
  /* Note to self: Why do I have dimensions + 1 tiles here? */
  tile_grid = (struct Drawable **)malloc(sizeof(struct Drawable *)
      * (mgs->map_window_x_chars + 1));
  int i;
  for (i = 0; i <= mgs->map_window_x_chars; i++) {
    tile_grid[i] = (struct Drawable *)malloc(sizeof(struct Drawable)
        * mgs->map_window_y_chars + 1);
  }
  get_tile_grid(map, mgs->map_window_x_chars, mgs->map_window_y_chars, mgs->draw_cursor, tile_grid);

  int x, y;
  Point write_coords;
  for (x = 0; x < mgs->map_window_x_chars; x++) {
    /* our x offset for writing is the top left corner of the visible window
     * plus the width of the characters witten so far.*/
    write_coords.x = map_window->x + (x  * mgs->at_width);
    for (y = 0; y < mgs->map_window_y_chars; y++) {
      write_coords.y = map_window->y + (y * mgs->line_height);
      /*DEBUG("Rendering %p from %d, %d at %d,%d\n", tile_grid[x][y].type, x , y,*/
          /*write_coords.x, write_coords.y);*/
      draw_sprite_at_point(&tile_grid[x][y], write_coords);
    }
  }

  for (i = 0; i < mgs->map_window_x_chars; i++) {
    free(tile_grid[i]);
  }
  free(tile_grid);
  return 1;
}
