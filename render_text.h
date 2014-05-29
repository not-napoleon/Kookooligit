#ifndef RENDER_TEXT_H_INCLUDED
#define RENDER_TEXT_H_INCLUDED

#include "SDL_ttf.h"

#include "graphics_wrapper.h"

/* TODO: Rename this to like RenderedWordList or some shit */
typedef struct SurfaceNode *SurfaceNodePtr;

typedef enum {
  scroll_up = -1,
  scroll_down = 1
} ScrollDir;

int build_word_list(const char *text, SurfaceNodePtr *dest, Color color,
    TTF_Font *font);
int get_message_height(SurfaceNodePtr msg, int w);
int render_message_to_window(const Rect *dstrect, SurfaceNodePtr text,
    int skip_line_height, int rows, ScrollDir dir);
void free_surface_list(SurfaceNodePtr slist);

#endif
