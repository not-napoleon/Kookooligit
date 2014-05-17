#include "SDL.h"
#include "SDL_ttf.h"

#include <stdbool.h>
#include <stdint.h>

#include <color_palette.h>
#include <graphics_wrapper.h>
#include <sprite.h>
#include <SDL_tools.h>

#define LOGGING_ENABLED
#include <log.h>

static bool _was_initilized = false;
static SDL_Texture *sprite_cache[SPRITE_TYPE_COUNT];
static TTF_Font *sprite_font = NULL;
static int at_width;
static int line_height;

void draw_sprite_at_point(const struct Drawable *drawable, const int x, const int y, const Rect *window) {
  /* x and y coordinates are given in tiles; the conversion to pixels happens here.*/
  if (!_was_initilized) {
    CRITICAL("Tried to draw an uninitilized sprite\n");
    exit(1);
  }
  SDL_Rect write_coords;
  /* our x offset for writing is the top left corner of the visible window
   * plus the width of the characters witten so far.*/
  write_coords.x = window->x + (x * at_width);
  write_coords.y = window->y + (y * line_height);
  write_coords.h = line_height;
  write_coords.w = at_width;

  /* TODO: Draw cursor if applicable */
  if (drawable->draw_cursor == 1) {
    DEBUG("Attempting to draw cursor\n");
    SDL_SetRenderDrawColor(get_main_renderer(), cursor_bg_color.r, cursor_bg_color.g, cursor_bg_color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(get_main_renderer(), &write_coords);
    SDL_SetRenderDrawColor(get_main_renderer(), map_bg_color.r, map_bg_color.g, map_bg_color.b, SDL_ALPHA_OPAQUE);
  }

  /* If the tile is unexplored, nothing left to do, leave it blank and go
   * home. */
  if (drawable->is_explored == 0) {
    return;
  }
  SDL_Texture *glyph = sprite_cache[drawable->sprite_id];
  /* Darken (which is to say make transparent) if the tile is unlit. */
  if (drawable->is_lit == 0) {
    SDL_SetTextureAlphaMod(glyph, 128);
  } else {
    SDL_SetTextureAlphaMod(glyph, 255);
  }

  /* Stage the texture for drawing */
  SDL_RenderCopy(get_main_renderer(), sprite_cache[drawable->sprite_id],
      NULL, &write_coords);

}

SDL_Texture *load_sprite_from_font(const char *glyph, const Color fg) {
  SDL_Surface *tmp = TTF_RenderText_Solid(sprite_font, glyph, convert_color(fg));
  SDL_Texture *retval = SDL_CreateTextureFromSurface(get_main_renderer(), tmp);
  SDL_FreeSurface(tmp);
  return retval;
}


void init_sprites_from_font(const char *font_path, const int point_size) {
  sprite_font = TTF_OpenFont(font_path, point_size);
  if (sprite_font == NULL) {
    ERROR( "Unable to load font %s: %s\n", font_path, SDL_GetError());
    exit(1);
  }
  if (TTF_FontFaceIsFixedWidth(sprite_font) == 0) {
    ERROR( "Font is not fixed width, chances are nothing will look right");
  }
  // Turn off kerning, since we want our characters to line up in a grid
  if (TTF_GetFontKerning(sprite_font) != 0) {
    TTF_SetFontKerning(sprite_font, 0);
  }
  // set font size data
  line_height = TTF_FontLineSkip(sprite_font);
  TTF_SizeText(sprite_font, "@", &at_width, NULL);
  sprite_cache[Player_sprite] = load_sprite_from_font("@", color_lit);
  sprite_cache[OffGrid_sprite] = load_sprite_from_font(" ", color_lit);
  sprite_cache[OpenSpace_sprite] = load_sprite_from_font(".", color_lit);
  sprite_cache[ImpassableWall_sprite] = load_sprite_from_font("#", color_lit);
  _was_initilized = true;
  DEBUG("Sprite graphics initilzed from front %s\n", font_path);
}

void close_sprites() {
  if (sprite_font != NULL) {
    TTF_CloseFont(sprite_font);
  }
}

int get_char_width(const int pixel_width) {
  return pixel_width / at_width;
}

int get_char_height(const int pixel_height) {
  return pixel_height / line_height;
}
