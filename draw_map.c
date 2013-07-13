#include "SDL.h"
#include "SDL_ttf.h"

#include <color_palette.h>
#include <draw_map.h>
#include <SDL_Tools.h>
#include <tile.h>

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

SDL_Surface *_render_map_glyph(const char *glyph, const Color fg, const Color bg) {
  return TTF_RenderText_Solid(get_map_font(), glyph, convert_color(fg));
}

int render_map_window(MapSection *map, MapGraphicsState *mgs, Rect *map_window,
    Point at_location, Point cursor_location){

  DEBUG("attempting to render map\n");
  SDL_Surface *screen = SDL_GetVideoSurface();
  if (screen == NULL) {
    CRITICAL("SDL_GetVideoSurface returned null!\n");
  }

  Point top_left;
  Point bottom_right;
  get_visible_region(map, mgs->map_window_x_chars, mgs->map_window_y_chars,
      &top_left, &bottom_right);

  // blank the screen
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {map_window->x, map_window->y, map_window->w, map_window->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Rect write_coords;
  int x, y;
  for (x = top_left.x; x <= bottom_right.x; x++) {
    // our x offset for writing is the top left corner of the visible window
    // plus the width of the characters witten so far.
    write_coords.x = map_window->x + ((x - top_left.x) * mgs->at_width);
    for (y = top_left.y; y <= bottom_right.y; y++) {
      write_coords.y = map_window->y + ((y - top_left.y) * mgs->line_height);
      SDL_Surface *map_char;
      if (x == cursor_location.x
          && y == cursor_location.y
          && mgs->draw_cursor == 1) {
        // Top priority - the cursor is always visible, if rendered at all
        DEBUG("Drawing Cursor\n");
        map_char = _render_map_glyph("*", color_lit, cursor_bg_color);
      } else if ( (x < 0) || (y < 0)
                      || (x >= map->x_size)
                      || (y >= map->y_size)
                      || map->matrix[x][y].is_explored == 0) {
        map_char = _render_map_glyph(tile_data[OffGrid]->glyph, color_hidden, map_bg_color);
      } else {
        if (x == at_location.x && y == at_location.y) {
          map_char = _render_map_glyph("@", color_lit, cursor_bg_color);
        } else {
          Color c;
          if (map->matrix[x][y].is_lit) {
            c = color_lit;
          } else {
            c = color_hidden;
          }
          map_char = _render_map_glyph(map->matrix[x][y].type->glyph, c,
              map_bg_color);
        }
      }
      SDL_BlitSurface(map_char, NULL, screen, &write_coords);
    }
  }

  SDL_UpdateRect(screen, map_window->x, map_window->y, map_window->w, map_window->h);

}
