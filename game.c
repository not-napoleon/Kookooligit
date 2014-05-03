#include <stdlib.h>
#include <game.h>
// We need tile to get the tile discriptor text for look mode.  Eventually
// that needs to get refactored into map.c
#include <tile.h>
#include <look.h>
#include <sprite.h>  /* for cleanup code */

#define LOGGING_ENABLED
#include <log.h>

GameState *allocate_game_state() {
  /*Allocate a new game state struct
   */
  GameState *state;
  state = malloc(sizeof(GameState));
  state->config = malloc(sizeof(GameConfiguration));
  state->messages = init_message_list();
  state->map = init_infinite_map();
  state->map_graphics_state = malloc(sizeof(MapGraphicsState));
  INFO("Allocated new game state\n");
  return state;
}

void free_game_state(GameState *state) {
  /* Recursively free GameState struct
   */
  free(state->config);
  free_message_queue(state->messages);
  free_infinite_map(state->map);
  free(state->map_graphics_state);
  close_sprites();
  free(state);
}

void process_command(GameState *state, CommandCode cmd) {
  TRACE("Processing Command %d - state is %d\n", cmd, state->state);
  if (cmd == NoOp) {
    return;
  }
  int delta_x = 0;
  int delta_y = 0;
  switch (cmd) {
    case EnterLookMode:
      DEBUG("Enter look mode\n");
      if (state->state == Move) {
        state->state = Look;
        /* TODO: Delegate this to map; Game shouldn't poke that deeply into the
         * map struct */
        state->map->cursor_location.x = state->map->at_location.x;
        state->map->cursor_location.y = state->map->at_location.y;
      }
      set_draw_cursor(state->map_graphics_state);
      state->need_to_redraw = 1;
      break;
    case ExitLookMode:
      DEBUG("Exiting look mode\n");
      if (state->state == Look) {
        state->state = Move;
      }
      clear_draw_cursor(state->map_graphics_state);
      state->need_to_redraw = 1;
      break;
    case MoveLeft:
      delta_x -= 1;
      DEBUG("Move left\n");
      break;
    case MoveRight:
      delta_x += 1;
      DEBUG("Move right\n");
      break;
    case MoveUp:
      delta_y -= 1;
      DEBUG("Move up\n");
      break;
    case MoveDown:
      delta_y += 1;
      DEBUG("Move down\n");
      break;
    case MoveUpLeft:
      delta_x -= 1;
      delta_y -= 1;
      DEBUG("Move up left\n");
      break;
    case MoveDownLeft:
      delta_x -= 1;
      delta_y += 1;
      DEBUG("Move down left\n");
      break;
    case MoveUpRight:
      delta_x += 1;
      delta_y -= 1;
      DEBUG("Move up right\n");
      break;
    case MoveDownRight:
      delta_x += 1;
      delta_y += 1;
      DEBUG("Move down right\n");
      break;
    case Quit:
      state->is_running = 0;
      break;
    case NoOp:
      /* This space intentionally left blank.  I know I could just let it fall
       * through, but it's a warning.*/
      break;
  }

  // Resolve charcter or cursor movement
  // TODO: This whole block probably belongs in map.c
  if (delta_x != 0 || delta_y != 0) {
    if (state->state == Move) {
      if (attempt_move(state->map, delta_x, delta_y)) {
        state->need_to_redraw = 1;
      } else {
        char tmp[50] = "You can't walk through walls";
        add_message(state->messages, tmp);
        state->need_to_redraw = 1;
      }
      calculate_visible_tiles(state->map, state->map->at_location);
    } else if (state->state == Look) {
      if (attempt_cursor_move(state->map, delta_x, delta_y,
            state->map_graphics_state->map_window_x_chars,
            state->map_graphics_state->map_window_y_chars)) {
        state->need_to_redraw = 1;
        // Get descriptor text
        Tile target_tile;
        target_tile = get_cursor_tile(state->map);
        const char* tile_desc;
        tile_desc = target_tile.type->description;
        DEBUG("got tile description %s\n", tile_desc);
        // draw look message
        if ( render_look_message(tile_desc, &state->config->status_window) == -1) {
          exit(-1);
        }
        // flag status window to redraw
        state->need_to_redraw = 1;
      }
    }
  }
}
