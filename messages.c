#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "messages.h"
#include "SDL_Tools.h"

#define LOG_LEVEL LOG_ERROR
#include "log.h"

static MessageList *queue;

void *init_message_list() {
  queue = malloc( sizeof(MessageList) );
  queue->first = NULL;
  queue->last = NULL;
  queue->length = 0;
  INFO("message list initilized\n");
}

void free_message(Message *message) {
  free_surface_list(message->rendered_words);
  free(message);
}


void free_message_queue() {
  while (queue->first != NULL) {
    MessageNodePtr temp = queue->first;
    queue->first = queue->first->next;
    free_message(temp->data);
    free(temp);
  }
  free(queue);
}

int add_message(char *text) {
  TTF_Font *font;
  font = get_message_font();
  DEBUG(" adding message with text <%s>\n", text);
  Message *msg = malloc( sizeof(Message) );
  // strtok will modify text
  msg->text = malloc( sizeof(char) * (strlen(text)+1));
  DEBUG("length of text is %d", strlen(text));
  strcpy(msg->text, text);
  msg->rendered_words = NULL;
  // Set the spacing data based on the font
  msg->skip_line_height = TTF_FontLineSkip(font);
  // Build the message object
  Color color = {255,255,255};

  build_word_list(text, &msg->rendered_words, color, font);

  // Build the message node
  MessageNodePtr new_node = malloc( sizeof(MessageNode) );
  new_node->data = msg;

  // Attach the message node to the message list
  new_node->next = queue->first;
  if (queue->first != NULL) {
    queue->first->prev = new_node;
  }
  queue->first = new_node;
  queue->length++;

  // TODO: trim to max length

}

int render_messages(const Rect *dstrect) {
  TRACE("rendering messages to x: %d, y: %d at w: %d and h: %d\n", dstrect->x,
      dstrect->y, dstrect->w, dstrect->h);
  //blank the message area, since we're going to redraw it
  MessageNodePtr curr;
  curr = queue->first;
  int h = dstrect->h;
  // While messages left & space left
  while (curr != NULL && h >= curr->data->skip_line_height) {
    int rows = get_message_height(curr->data->rendered_words, dstrect->w);
    Rect write_coords;
    write_coords.x = dstrect->x;
    write_coords.y = dstrect->y + h;
    write_coords.w = dstrect->w;
    write_coords.h = dstrect->h;

    DEBUG("Rendering message %s\n", curr->data->text);
    render_message_to_window(&write_coords, curr->data->rendered_words,
        curr->data->skip_line_height, rows, scroll_up);

    h -= rows * curr->data->skip_line_height;
    DEBUG("message rendered in %i rows\n", rows);
    curr = curr->next;
  }
  DEBUG("all messages rendered, updateing screen\n");
}
