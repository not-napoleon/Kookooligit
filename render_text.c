#include <render_text.h>

#define LOGGING_ENABLED
#include <log.h>
/*
 * Screen list
 */

SurfaceNodePtr make_surface_node(SDL_Surface *surface) {
  SurfaceNodePtr node = malloc( sizeof(SurfaceNode) );
  node->surface = surface;
  node->next = NULL;
  return node;
}

void free_surface_list(SurfaceNodePtr slist) {
  while (slist != NULL) {
    SurfaceNodePtr tmp = slist;
    slist = slist->next;
    SDL_FreeSurface(tmp->surface);
    free(tmp);
  }
}

int build_word_list(char *text, SurfaceNodePtr *dest, SDL_Color color,
    TTF_Font *font) {
  char *curr_word = strtok(text, " ");
  SurfaceNodePtr curr_surface;
  SurfaceNodePtr prev_surface = NULL;
  do {
    // Append a space to the word, since we clobbered it in tokenization
    char *tmp = malloc( sizeof(char) * (strlen(curr_word) + 1));
    strcpy(tmp, curr_word);
    strcat(tmp, " ");
    curr_surface = make_surface_node( TTF_RenderText_Solid(font, tmp, color) );
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
  SurfaceNodePtr curr = msg;
  int line_width = 0;
  int line_rows = 1; // how many rows of text at this width
  while (curr != NULL) {
    if (line_width + curr->surface->w > w) {
      line_rows += 1;
      line_width = curr->surface->w;
    } else {
      line_width += curr->surface->w;
    }
    curr = curr->next;
  }
  return line_rows;
}

int render_message_to_window(const SDL_Rect *dstrect, SDL_Surface *screen,
    SurfaceNodePtr text, int skip_line_height, int rows) {
  TRACE("rendering text to x: %d, y: %d at w: %d and h: %d\n", dstrect->x,
      dstrect->y, dstrect->w, dstrect->h);
  int h_offset = rows * skip_line_height;
  SurfaceNodePtr curr_word = text;
  int line_width = 0;
  // assemble the next message
  while (curr_word != NULL) {
    SDL_Rect write_coords;
    if (line_width + curr_word->surface->w > dstrect->w) {
      // line wrap - set the line width to 0 and move the height down
      line_width = 0;
      h_offset -= skip_line_height;
    }
    write_coords.x = dstrect->x + line_width;
    write_coords.y = dstrect->y - h_offset;
    DEBUG("writing at %i, %i\n", write_coords.x, write_coords.y);
    //DEBUG("h is %i, h_offset is %i\n", h, h_offset);
    SDL_BlitSurface(curr_word->surface, NULL, screen, &write_coords);
    line_width += curr_word->surface->w;
    curr_word = curr_word->next;
  }
}
