#include <SDL_Tools.h>

#define LOGGING_ENABLED
#include <log.h>

SDL_Color convert_color(Color c) {
  SDL_Color sc;
  sc.r = c.r;
  sc.g = c.g;
  sc.b = c.b;

  return sc;
}

TTF_Font *message_font = NULL;
TTF_Font *status_font = NULL;
TTF_Font *command_font = NULL;

void set_message_font(const char *font_path, const int point_size) {
  message_font = TTF_OpenFont(font_path, point_size);
  if (message_font == NULL) {
    ERROR( "Unable to load font %s: %s\n", font_path, SDL_GetError());
    exit(1);
  }
}

void set_status_font(const char *font_path, const int point_size) {
  status_font = TTF_OpenFont(font_path, point_size);
  if (status_font == NULL) {
    ERROR( "Unable to load font %s: %s\n", font_path, SDL_GetError());
    exit(1);
  }
}

void set_command_font(const char *font_path, const int point_size) {
  command_font = TTF_OpenFont(font_path, point_size);
  if (command_font == NULL) {
    ERROR( "Unable to load font %s: %s\n", font_path, SDL_GetError());
    exit(1);
  }
}


TTF_Font* get_message_font() { return message_font; }
TTF_Font* get_status_font() { return status_font; }
TTF_Font* get_command_font() { return command_font; }

void close_fonts() {
  if (message_font != NULL) {
    TTF_CloseFont(message_font);
  }
  if (status_font != NULL) {
    TTF_CloseFont(status_font);
  }
  if (command_font != NULL) {
    TTF_CloseFont(command_font);
  }
}
