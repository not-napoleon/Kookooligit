#include "SDL.h"

#include <color_palette.h>
#include <draw_map.h>
#include <SDL_Tools.h>
#include <tile.h>

//#define LOGGING_ENABLED
#include <log.h>

typedef struct tile_data {
  SDL_Surface *lit_graphic;
  SDL_Surface *hidden_graphic;
} tile_data;
/*
 * File global to store the rendered map graphics
 */
static short int was_initilized = 0;
static SDL_Surface *rendered_at;
static SDL_Surface *rendered_cursor;
static tile_data MapGraphics[TILE_TYPE_COUNT];

int init_map_graphics(TTF_Font *font) {
  /*
   * pre-render the map graphics
   */

  rendered_at = TTF_RenderText_Solid(font, "@", convert_color(color_lit));
  rendered_cursor = TTF_RenderText_Solid(font, "*", convert_color(color_lit));

  MapGraphics[OffGrid].lit_graphic = TTF_RenderText_Solid(font, " ",
      convert_color(color_lit));
  MapGraphics[OpenSpace].lit_graphic = TTF_RenderText_Solid(font, ".",
      convert_color(color_lit));
  MapGraphics[ImpassableWall].lit_graphic = TTF_RenderText_Solid(font, "#",
      convert_color(color_lit));

  MapGraphics[OffGrid].hidden_graphic = TTF_RenderText_Solid(font, " ",
      convert_color(color_hidden));
  MapGraphics[OpenSpace].hidden_graphic = TTF_RenderText_Solid(font, ".",
      convert_color(color_hidden));
  MapGraphics[ImpassableWall].hidden_graphic = TTF_RenderText_Solid(font, "#",
      convert_color(color_hidden));

  was_initilized = 1;
  INFO("Map graphics initilized\n");
}

void set_draw_cursor(MapGraphicsState *mgs) {
  mgs->draw_cursor = 1;
}

void clear_draw_cursor(MapGraphicsState *mgs) {
  mgs->draw_cursor = 0;
}

int render_map_window(MapSection *map, MapGraphicsState *mgs, Rect *map_window,
    Point at_location, Point cursor_location){

  SDL_Surface *screen = SDL_GetVideoSurface();
  if (screen == NULL) {
    CRITICAL("SDL_GetVideoSurface returned null!\n");
  }

  Point top_left;
  Point bottom_right;
  get_visible_region(map, mgs->map_window_x_chars, mgs->map_window_y_chars,
      &top_left, &bottom_right);

  DEBUG("attempting to render map\n");
  if (was_initilized == 0) {
    ERROR("Map was not initilized\n");
    return -1;
  }
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
        map_char = rendered_cursor;
      } else if ( (x < 0) || (y < 0)
                      || (x >= MAP_SECTION_SIZE)
                      || (y >= MAP_SECTION_SIZE)
                      || map->matrix[x][y].is_explored == 0) {
        map_char = MapGraphics[OffGrid].lit_graphic;
      } else {
        if (x == at_location.x && y == at_location.y) {
          map_char = rendered_at;
        } else {
          if (map->matrix[x][y].is_lit) {
            map_char = MapGraphics[map->matrix[x][y].type].lit_graphic;
          } else {
            map_char = MapGraphics[map->matrix[x][y].type].hidden_graphic;
          }
        }
      }
      SDL_BlitSurface(map_char, NULL, screen, &write_coords);
    }
  }

  SDL_UpdateRect(screen, map_window->x, map_window->y, map_window->w, map_window->h);

}
