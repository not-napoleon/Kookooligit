#include <map.h>

#include "SDL.h"
#include "SDL_ttf.h"

int is_passable_point(MapSection *map, Point p) {
  return map->matrix[p.x][p.y].is_passable;
}

/*
 * File global to store the rendered map graphics
 */
static short int was_initilized = 0;
static SDL_Surface *rendered_at;
static SDL_Surface *MapGraphics[TILE_TYPE_COUNT];

int init_map_graphics(TTF_Font *font) {
  /*
   * pre-render the map graphics
   */
  SDL_Color color = {255,255,255};

  rendered_at = TTF_RenderText_Solid(font, "@", color);

  MapGraphics[OffGrid] = TTF_RenderText_Solid(font, " ", color);
  MapGraphics[OpenSpace] = TTF_RenderText_Solid(font, ".", color);
  MapGraphics[ImpassableWall] = TTF_RenderText_Solid(font, "#", color);

  was_initilized = 1;
  printf("Map graphics initilized\n");
}

int generate_map(MapSection *map) {
  int x;
  int y;
  for(x = 0; x < MAP_SECTION_SIZE; x++) {
    for(y = 0; y < MAP_SECTION_SIZE; y++) {
      if( ((((x/8) % 2) == 0) && (((y/8) % 2) == 0))
       || ( x == 0) || (x == 63) || (y == 0) || (y == 63) ){
        map->matrix[x][y].type = ImpassableWall;
        map->matrix[x][y].is_passable = 0;
      } else {
        map->matrix[x][y].type = OpenSpace;
        map->matrix[x][y].is_passable = 1;
      }
      map->matrix[x][y].is_explored = 0;
      map->matrix[x][y].contains_player = 0;
    }
  }
  printf("Map generated\n");
  return 0;
}

int draw_map(MapSection *map, TTF_Font *font,
    SDL_Surface *screen, const SDL_Rect *map_window) {
  /*
   * map - the map we're drawing
   * font - the font to render the map in
   * screen - the surface to render the map to
   * map_window - the space allocated for the map on the given surface
   */
  printf("attempting to render map\n");
  if (was_initilized == 0) {
    printf("Map was not initilized\n");
    return -1;
  }
  SDL_Color color = {255,255,255};

  int char_height = TTF_FontLineSkip(font);

  // figure out viewable portion of map
  // We're depending on fixed width fonts here...
  int window_x_chars, window_y_chars;
  window_x_chars = map_window->w / rendered_at->w;
  window_y_chars = map_window->h / char_height;
  printf("window size in characters is %i by %i\n", window_x_chars,
      window_y_chars);

  Point top_left;
  top_left.x = map->center.x - (window_x_chars / 2);
  top_left.y = map->center.y - (window_y_chars / 2);

  Point bottom_right;
  bottom_right.x = map->center.x + (window_x_chars / 2);
  bottom_right.y = map->center.y + (window_y_chars / 2);

  printf("rendering character section from (%i, %i) to (%i, %i)\n",
      top_left.x, top_left.y, bottom_right.x, bottom_right.y);

  // blank the screen
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {map_window->x, map_window->y, map_window->w, map_window->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Rect write_coords;
  int x, y;
  for (x = top_left.x; x <= bottom_right.x; x++) {
    // our x offset for writing is the top left corner of the visible window
    // plus the width of the characters witten so far.
    write_coords.x = map_window->x + ((x - top_left.x) * rendered_at->w);
    for (y = top_left.y; y <= bottom_right.y; y++) {
      write_coords.y = map_window->y + ((y - top_left.y) * char_height);
      SDL_Surface *map_char;
      if ( (x < 0) || (y < 0) || (x >= MAP_SECTION_SIZE)
          || (y >= MAP_SECTION_SIZE) ) {
        map_char = MapGraphics[OffGrid];
      } else {
        if (map->matrix[x][y].contains_player == 1) {
          map_char = rendered_at;
        } else {
          map_char = MapGraphics[map->matrix[x][y].type];
        }
      }
      SDL_BlitSurface(map_char, NULL, screen, &write_coords);
    }
  }

  SDL_UpdateRect(screen, map_window->x, map_window->y, map_window->w, map_window->h);

  return 0;
}
