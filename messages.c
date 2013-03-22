#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
/*
 * Screen list
 *
 * this will probably need to get refactored eventually
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

/*
 * Message logic here
 */


MessageList *init_message_list() {
  MessageList *mlist;
  mlist = malloc( sizeof(MessageList) );
  mlist->first = NULL;
  mlist->last = NULL;
  mlist->length = 0;
  return mlist;
}


int add_message(MessageList *mlist, char *text, TTF_Font *font) {
  Message *msg = malloc( sizeof(Message) );
  msg->text = text;
  msg->rendered_words = NULL;
  // Set the spacing data based on the font
  msg->skip_line_height = TTF_FontLineSkip(font);
  TTF_SizeText(font, " ", &msg->space_width, NULL);
  // Build the message object
  SDL_Color color = {255,255,255};
  char *curr_word = strtok(text, " ");
  SurfaceNodePtr curr_surface;
  SurfaceNodePtr prev_surface = NULL;
  do {
    curr_surface = make_surface_node( TTF_RenderText_Solid(font, curr_word, color) );
    if (curr_surface->surface == NULL) {
      fprintf(stderr, "Unable to draw text <<%s>>: %s\n", curr_word, SDL_GetError());
      return -1;
    }
    // Don't lose the start of the list
    if (msg->rendered_words == NULL) {
      msg->rendered_words = curr_surface;
    }
    if (prev_surface != NULL) {
      prev_surface->next = curr_surface;
    }
    prev_surface = curr_surface;
    curr_word = strtok(NULL, " ");
  } while ( curr_word != NULL);


  // Build the message node
  MessageNodePtr new_node = malloc( sizeof(MessageNode) );
  new_node->data = msg;

  // Attach the message node to the message list
  new_node->next = mlist->first;
  if (mlist->first != NULL) {
    mlist->first->prev = new_node;
  }
  mlist->first = new_node;
  mlist->length++;

  // TODO: trim to max length

}

int get_message_height(Message *msg, int w) {
  SurfaceNodePtr curr = msg->rendered_words;
  int line_width = 0;
  int line_rows = 0; // how many rows of text at this width
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

int render_messages(int x, int y, int w, int h, SDL_Surface *screen,
    MessageList *mlist) {
  printf("Rendering messages\n");
  MessageNodePtr curr;
  curr = mlist->first;
  while (curr != NULL && h > 0) {
  // While messages left & space left
    printf("Rendering message %s", curr->data->text);
    int rows = get_message_height(curr->data, w);
    int h_offset = rows * curr->data->skip_line_height;
    SurfaceNodePtr curr_word = curr->data->rendered_words;
    int line_width = 0;
    // assemble the next message
    while (curr_word != NULL) {
      printf("rendering another word\n");
      SDL_Rect *write_coords;
      if (line_width + curr_word->surface->w > w) {
        // line wrap - set the line width to 0 and move the height down
        line_width = 0;
        h_offset -= curr->data->skip_line_height;
      }
      write_coords->x = x + line_width;
      write_coords->y = y + (h - h_offset);
      SDL_BlitSurface(curr_word->surface, NULL, screen, write_coords);
      line_width += curr_word->surface->w;
    }
    printf("message rendered\n");
    curr = curr->next;
    h -= rows * curr->data->skip_line_height;
  }
  SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

void free_message_queue(MessageList *queue) {
  while (queue->first != NULL) {
    MessageNodePtr temp = queue->first;
    queue->first = queue->first->next;
    free(temp);
  }
  free(queue);
}

void free_message(Message *message) {
  free_surface_list(message->rendered_words);
  free(message);
}
