#include "SDL.h"
#include "SDL_ttf.h"

#include <SDL_Tools.h>
#include <color_palette.h>
#include <look.h>
#include <render_text.h>

#define LOGGING_ENABLED
#include <log.h>

int render_look_message(const char* message, const Rect *dstrect) {
  if (message == NULL) {
    WARN("Ignoring NULL look message\n");
    return 0;
  }
  TTF_Font *font;
  font = get_status_font();
  DEBUG("rendering look message <%s>\n", message);
  SurfaceNodePtr rendered_message = NULL;
  if (build_word_list(message, &rendered_message, color_default_text, font) == -1) {
    return -1;
  }
  DEBUG("Built word list\n");
  int rows;
  rows = get_message_height(rendered_message, dstrect->w);
  if (render_message_to_window(dstrect, rendered_message, TTF_FontLineSkip(font), rows, scroll_down) == -1) {
    ERROR("Failed to render look message to window!\n");
    return -1;
  }
  DEBUG("Message rendered, updating screen\n");
  return 0;
}

