#include <map.h>


int draw_map(Point at_location, TTF_Font *font, SDL_Surface *screen, const SDL_Rect *map_window) {
  SDL_Color color = {255,255,255};

  const char* at = "@";
  int char_height = TTF_FontLineSkip(font);

  SDL_Surface *rendered_at;
  rendered_at = TTF_RenderText_Solid(font, at, color);
  int x_offset = at_location.x * rendered_at->w;
  int y_offset = at_location.y * char_height;

  // blank the screen
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {map_window->x, map_window->y, map_window->w, map_window->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));

  SDL_Rect write_coords;
  write_coords.x = map_window->x + x_offset;
  write_coords.y = map_window->y + y_offset;
  SDL_BlitSurface(rendered_at, NULL, screen, &write_coords);
  SDL_UpdateRect(screen, map_window->x, map_window->y, map_window->w, map_window->h);

  return 0;
}
