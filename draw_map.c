#include <draw_map.h>
typedef struct tile_data {
  SDL_Surface *lit_graphic;
  SDL_Surface *hidden_graphic;
} tile_data;
/*
 * File global to store the rendered map graphics
 */
static short int was_initilized = 0;
static SDL_Surface *rendered_at;
static tile_data MapGraphics[TILE_TYPE_COUNT];



int init_map_graphics(TTF_Font *font) {
  /*
   * pre-render the map graphics
   */
  SDL_Color color_lit = {255,255,255};
  SDL_Color color_hidden = {127,127,127};

  rendered_at = TTF_RenderText_Solid(font, "@", color_lit);

  MapGraphics[OffGrid].lit_graphic = TTF_RenderText_Solid(font, " ", color_lit);
  MapGraphics[OpenSpace].lit_graphic = TTF_RenderText_Solid(font, ".", color_lit);
  MapGraphics[ImpassableWall].lit_graphic = TTF_RenderText_Solid(font, "#", color_lit);

  MapGraphics[OffGrid].hidden_graphic = TTF_RenderText_Solid(font, " ", color_hidden);
  MapGraphics[OpenSpace].hidden_graphic = TTF_RenderText_Solid(font, ".", color_hidden);
  MapGraphics[ImpassableWall].hidden_graphic = TTF_RenderText_Solid(font, "#", color_hidden);

  was_initilized = 1;
  printf("Map graphics initilized\n");
}

int render_map_window(MapSection *map, Point *top_left, Point *bottom_right,
    SDL_Surface *screen, SDL_Rect *map_window, int at_width, int line_height) {

  printf("attempting to render map\n");
  if (was_initilized == 0) {
    printf("Map was not initilized\n");
    return -1;
  }
  SDL_Color color = {255,255,255};
  // blank the screen
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {map_window->x, map_window->y, map_window->w, map_window->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Rect write_coords;
  int x, y;
  for (x = top_left->x; x <= bottom_right->x; x++) {
    // our x offset for writing is the top left corner of the visible window
    // plus the width of the characters witten so far.
    write_coords.x = map_window->x + ((x - top_left->x) * at_width);
    for (y = top_left->y; y <= bottom_right->y; y++) {
      write_coords.y = map_window->y + ((y - top_left->y) * line_height);
      SDL_Surface *map_char;
      if ( (x < 0) || (y < 0) || (x >= MAP_SECTION_SIZE)
          || (y >= MAP_SECTION_SIZE) ) {
        map_char = MapGraphics[OffGrid].lit_graphic;
      } else {
        if (map->matrix[x][y].contains_player == 1) {
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
