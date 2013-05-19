#include <color_palette.h>
#include <look.h>
#include <render_text.h>

#define LOGGING_ENABLED
#include <log.h>

int render_look_message(const char* message, SDL_Surface *screen,
    const Rect *dstrect, TTF_Font *font) {
  DEBUG("rendering look message <%s>\n", message);
  clear_rect(dstrect);
  SurfaceNodePtr rendered_message = NULL;
  if (build_word_list(message, &rendered_message, color_default_text, font) == -1) {
    return -1;
  }
  DEBUG("Built word list\n");
  int rows;
  rows = get_message_height(rendered_message, dstrect->w);
  if (render_message_to_window(dstrect, screen, rendered_message, TTF_FontLineSkip(font), rows, scroll_down) == -1) {
    return -1;
  }
  DEBUG("Message rendered, updating screen\n");
  SDL_UpdateRect(screen, dstrect->x, dstrect->y, dstrect->w, dstrect->h);
}

