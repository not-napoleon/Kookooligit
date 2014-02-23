#include "SDL.h"
#include "SDL_ttf.h"

#include <color_palette.h>
#include <draw_map.h>
#include <SDL_Tools.h>
#include <tile.h>

/*#define LOGGING_ENABLED*/
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

SDL_Surface *_render_map_glyph(const char *glyph, const Color fg, const Color bg) {
  return TTF_RenderText_Solid(get_map_font(), glyph, convert_color(fg));
}

int render_map_window(InfiniteMap *map, MapGraphicsState *mgs, Rect *map_window){

  DEBUG("attempting to render map - window size is %d x %d (line_height: %d, at_width: %d)\n",
      mgs->map_window_x_chars, mgs->map_window_y_chars, mgs->line_height, mgs->at_width);
  SDL_Surface *screen = SDL_GetVideoSurface();
  if (screen == NULL) {
    CRITICAL("SDL_GetVideoSurface returned null!\n");
  }

  Point at_location;
  Point cursor_location;
  Tile **tile_grid;
  tile_grid = (Tile **)malloc(sizeof(Tile *) * (mgs->map_window_x_chars + 1));
  int i;
  for (i = 0; i <= mgs->map_window_x_chars; i++) {
    tile_grid[i] = (Tile *)malloc(sizeof(Tile) * mgs->map_window_y_chars + 1);
  }
  get_tile_grid(map, mgs->map_window_x_chars, mgs->map_window_y_chars,
      &at_location, &cursor_location, tile_grid);

  // blank the screen
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {map_window->x, map_window->y, map_window->w, map_window->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Rect write_coords;
  int x, y;
  for (x = 0; x < mgs->map_window_x_chars; x++) {
    /* our x offset for writing is the top left corner of the visible window
     * plus the width of the characters witten so far.*/
    write_coords.x = map_window->x + (x  * mgs->at_width);
    for (y = 0; y < mgs->map_window_y_chars; y++) {
      write_coords.y = map_window->y + (y * mgs->line_height);
      /*DEBUG("Rendering %p from %d, %d at %d,%d\n", tile_grid[x][y].type, x , y,*/
          /*write_coords.x, write_coords.y);*/
      SDL_Surface *map_char;
      if (x == cursor_location.x
          && y == cursor_location.y
          && mgs->draw_cursor == 1) {
        // Top priority - the cursor is always visible, if rendered at all
        DEBUG("Rendering Cursor\n");
        map_char = _render_map_glyph("*", color_lit, cursor_bg_color);
      } else if ( tile_grid[x][y].is_explored == 0) {
        map_char = _render_map_glyph(tile_data[OffGrid]->glyph, color_hidden, map_bg_color);
      } else {
        if (x == at_location.x && y == at_location.y) {
          DEBUG("Rendering at\n");
          map_char = _render_map_glyph("@", color_lit, cursor_bg_color);
        } else {
          Color c;
          if (tile_grid[x][y].is_lit) {
            c = color_lit;
          } else {
            c = color_hidden;
          }
          map_char = _render_map_glyph(tile_grid[x][y].type->glyph, c,
              map_bg_color);
        }
      }
      SDL_BlitSurface(map_char, NULL, screen, &write_coords);
    }
  }

  for (i = 0; i < mgs->map_window_x_chars; i++) {
    free(tile_grid[i]);
  }
  free(tile_grid);
  SDL_UpdateRect(screen, map_window->x, map_window->y, map_window->w, map_window->h);
}
