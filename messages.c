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
  printf("message list initilized\n");
  return mlist;
}


int add_message(MessageList *mlist, char *text, TTF_Font *font) {
  printf(" adding message with text %s", text);
  Message *msg = malloc( sizeof(Message) );
  // strtok will modify text
  msg->text = malloc( sizeof(char) * strlen(text));
  strcpy(msg->text, text);
  msg->rendered_words = NULL;
  // Set the spacing data based on the font
  msg->skip_line_height = TTF_FontLineSkip(font);
  // Build the message object
  SDL_Color color = {255,255,255};
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

int render_messages(const SDL_Rect *dstrect, SDL_Surface *screen,
    MessageList *mlist) {
  printf("Rendering messages\n");
  //blank the message area, since we're going to redraw it
  // Fill rect can change the dest rect for clipping, so pass in a copy
  SDL_Rect tmp = {dstrect->x, dstrect->y, dstrect->w, dstrect->h};
  SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));
  MessageNodePtr curr;
  curr = mlist->first;
  int h = dstrect->h;
  while (curr != NULL && h >= curr->data->skip_line_height) {
  // While messages left & space left
    printf("Rendering message %s\n", curr->data->text);
    int rows = get_message_height(curr->data, dstrect->w);
    int h_offset = rows * curr->data->skip_line_height;
    SurfaceNodePtr curr_word = curr->data->rendered_words;
    int line_width = 0;
    // assemble the next message
    while (curr_word != NULL) {
      SDL_Rect write_coords;
      if (line_width + curr_word->surface->w > dstrect->w) {
        // line wrap - set the line width to 0 and move the height down
        line_width = 0;
        h_offset -= curr->data->skip_line_height;
      }
      write_coords.x = dstrect->x + line_width;
      write_coords.y = dstrect->y + (h - h_offset);
      printf("writing at %i, %i\n", write_coords.x, write_coords.y);
      printf("h is %i, h_offset is %i\n", h, h_offset);
      SDL_BlitSurface(curr_word->surface, NULL, screen, &write_coords);
      line_width += curr_word->surface->w;
      curr_word = curr_word->next;
    }
    printf("message rendered in %i rows\n", rows);
    h -= rows * curr->data->skip_line_height;
    curr = curr->next;
  }
  printf("all messages rendered, updateing screen\n");
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
