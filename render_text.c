#include "SDL.h"

#include "render_text.h"
#include "SDL_Tools.h"

/*#define LOGGING_ENABLED*/
#include "log.h"
/*
 * Screen list
 */

/* TODO: Use a utlist here too */
typedef struct SurfaceNode {
  SDL_Texture *surface;
  uint32_t w;
  SurfaceNodePtr next;
} SurfaceNode;

SurfaceNodePtr make_surface_node(SDL_Surface *surface) {
  SurfaceNodePtr node = malloc( sizeof(SurfaceNode) );
  node->surface =
    SDL_CreateTextureFromSurface(get_main_renderer(), surface);
  node->w = surface->w;
  node->next = NULL;
  return node;
}

void free_surface_list(SurfaceNodePtr slist) {
  while (slist != NULL) {
    SurfaceNodePtr tmp = slist;
    slist = slist->next;
    SDL_DestroyTexture(tmp->surface);
    free(tmp);
  }
}

int build_word_list(const char *text, SurfaceNodePtr *dest, Color color,
    TTF_Font *font) {
  SDL_Color c;
  c = convert_color(color);
  char *mutable_text = malloc( sizeof(char) * (strlen(text) + 1));
  strcpy(mutable_text, text);
  char *curr_word = strtok(mutable_text, " ");
  SurfaceNodePtr curr_surface;
  SurfaceNodePtr prev_surface = NULL;
  do {
    // Append a space to the word, since we clobbered it in tokenization
    char *tmp = malloc( sizeof(char) * (strlen(curr_word) + 2));
    strcpy(tmp, curr_word);
    strcat(tmp, " ");
    SDL_Surface *tmp_surface = TTF_RenderText_Solid(font, tmp, c);
    curr_surface = make_surface_node(tmp_surface);
    SDL_FreeSurface(tmp_surface);
    free(tmp);
    if (curr_surface->surface == NULL) {
      ERROR("Unable to draw text <<%s>>: %s\n", curr_word, SDL_GetError());
      return -1;
    }
    // Don't lose the start of the list
    if (*dest == NULL) {
      *dest = curr_surface;
    }
    if (prev_surface != NULL) {
      prev_surface->next = curr_surface;
    }
    prev_surface = curr_surface;
    curr_word = strtok(NULL, " ");
  } while ( curr_word != NULL);
}

int get_message_height(SurfaceNodePtr msg, int w) {
  TRACE("Getting message hight\n");
  SurfaceNodePtr curr = msg;
  int line_width = 0;
  int line_rows = 1; // how many rows of text at this width
  while (curr != NULL) {
    if (line_width + curr->w > w) {
      line_rows += 1;
      line_width = curr->w;
    } else {
      line_width += curr->w;
    }
    curr = curr->next;
  }
  return line_rows;
}

int render_message_to_window(const Rect *dstrect, SurfaceNodePtr text,
    int skip_line_height, int rows, ScrollDir dir) {
  TRACE("rendering text to x: %d, y: %d at w: %d and h: %d\n", dstrect->x,
      dstrect->y, dstrect->w, dstrect->h);
  SDL_Rect r = {dstrect->x, dstrect->y, dstrect->w, dstrect->h};
  int h_offset;
  if (dir == scroll_down) {
   h_offset = 0;
  } else {
    h_offset = rows * skip_line_height;
  }
  SurfaceNodePtr curr_word = text;
  int line_width = 0;
  // assemble the next message
  while (curr_word != NULL) {
    SDL_Rect write_coords;
    if (line_width + curr_word->w > r.w) {
      // line wrap - set the line width to 0 and move the height down
      line_width = 0;
      h_offset += skip_line_height * dir;
    }
    write_coords.x = r.x + line_width;
    write_coords.y = r.y + (dir * h_offset);
    write_coords.w = curr_word->w;
    write_coords.h = skip_line_height;
    DEBUG("writing at %i, %i\n", write_coords.x, write_coords.y);
    //DEBUG("h is %i, h_offset is %i\n", h, h_offset);
    SDL_RenderCopy(get_main_renderer(), curr_word->surface, NULL, &write_coords);
    line_width += curr_word->w;
    curr_word = curr_word->next;
  }
}
