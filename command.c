#include "SDL.h"
#include <command.h>
#include <command_list.h>
#include <lib/uthash/src/uthash.h>

//#define LOGGING_ENABLED
#include <log.h>

struct KeyToCommand {
  SDL_Keycode key;
  /* TODO: Allow mapping multiple commands to a key
   * specifically, this should eventually be a sub-hash based on
   * command modes, which currently don't really exist
   */
  enum CommandCode command;
  UT_hash_handle hh;            /* Make this hashable */
};

const int KEY_SIZE = sizeof(SDL_Keycode);

/*
 * File global key mapping hash
 */
struct KeyToCommand *key_mapping = NULL;

int bind_command_key_code(enum CommandCode command, const SDL_Keycode key) {
  /*
   * Bind the given key code to the given command
   */

  struct KeyToCommand *new_key_map;

  /* Check if the key exists in the hash, and return an error if it does */

  HASH_FIND(hh, key_mapping, &key, KEY_SIZE, new_key_map);
  if (new_key_map == NULL) {
    /* create key mapping instance */
    new_key_map = (struct KeyToCommand*)malloc(sizeof(struct KeyToCommand));
    new_key_map->key = key;
    new_key_map->command = command;

    /* Add new key map to hash */
    HASH_ADD(hh, key_mapping, key, KEY_SIZE, new_key_map);

    /* TODO: Add reverse mapping command -> key */

    return 0;
  } else {
    /* Key exists error */
    return -1;
  }
}

int bind_command_name(enum CommandCode command, const char *symbol) {
  /*
   * Bind the given symbol to the given command code
   */
  return bind_command_key_code(command, SDL_GetKeyFromName(symbol));
}


void free_command_mapping() {
  /*
   * Clean up the internal command mapping data structures
   */
  struct KeyToCommand *curr, *tmp;

  HASH_ITER(hh, key_mapping, curr, tmp) {
    HASH_DEL(key_mapping, curr);
    free(curr);
  }
}

enum CommandCode parse_keypress(const SDL_Event event) {
  struct KeyToCommand *pressed;
  HASH_FIND(hh, key_mapping, &event.key.keysym.sym, KEY_SIZE, pressed);
  if (pressed == NULL) {
    /*
     * Unmapped key
     */
    return NoOp;
  } else {
    return pressed->command;
  }
}


enum CommandCode get_command() {
  /* Just call parse keypress if it's a keypress, and ignore it otherwise
   */
  SDL_Event event;
  SDL_WaitEvent(&event);
  enum CommandCode cmd;
  switch (event.type) {
    case SDL_KEYDOWN:
      TRACE("got keypress event\n");
      cmd = parse_keypress(event);
      break;
    case SDL_QUIT:
      TRACE("Got quit signal by magic\n");
      cmd = Quit;
      break;
    default:
      WARN("Unknown event type %d\n", event.type);
      cmd = NoOp;
      break;
  }
  return cmd;
}
