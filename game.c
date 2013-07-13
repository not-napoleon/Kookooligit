#include <game.h>
// We need tile to get the tile discriptor text for look mode.  Eventually
// that needs to get refactored into map.c
#include <tile.h>

#define LOGGING_ENABLED
#include <log.h>

GameState *allocate_game_state() {
  /*Allocate a new game state struct
   */
  GameState *state;
  state = malloc(sizeof(GameState));
  state->config = malloc(sizeof(GameConfiguration));
  state->messages = init_message_list();
  state->map = init_map_section();
  state->map_graphics_state = malloc(sizeof(MapGraphicsState));
  INFO("Allocated new game state\n");
  return state;
}

void free_game_state(GameState *state) {
  /* Recursively free GameState struct
   */
  free(state->config);
  free_message_queue(state->messages);
  free(state->map);
  free(state->map_graphics_state);
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
        state->cursor_location.x = state->at_location.x;
        state->cursor_location.y = state->at_location.y;
      }
      set_draw_cursor(state->map_graphics_state);
      state->need_to_redraw_map = 1;
      break;
    case ExitLookMode:
      DEBUG("Exiting look mode\n");
      if (state->state == Look) {
        state->state = Move;
      }
      clear_draw_cursor(state->map_graphics_state);
      state->need_to_redraw_map = 1;
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
  }

  // Resolve charcter or cursor movement
  // TODO: This whole block probably belongs in map.c
  if (delta_x != 0 || delta_y != 0) {

    Point target_point;
    if (state->state == Move) {
      target_point = state->at_location;
    } else if (state->state == Look) {
      target_point = state->cursor_location;
    }

    target_point.x = target_point.x + delta_x;
    target_point.y = target_point.y + delta_y;

    if (state->state == Move) {
      if (is_passable_point(state->map, target_point) == 1) {
        state->at_location = target_point;
        state->need_to_redraw_map = 1;
      } else {
        char tmp[50] = "You can't walk through walls";
        add_message(state->messages, tmp);
        state->need_to_redraw_messages = 1;
      }
      // Decide if we need to recenter the map section
      if (abs(state->at_location.x - state->map->center.x) > 10) {
        state->map->center.x = state->at_location.x;
      }
      if (abs(state->at_location.y - state->map->center.y) > 10) {
        state->map->center.y = state->at_location.y;
      }
      DEBUG("at_location (%i, %i), center (%i, %i)\n", state->at_location.x,
          state->at_location.y, state->map->center.x, state->map->center.y);
      calculate_visible_tiles(state->map, state->at_location);
    } else if (state->state == Look) {
      Point top_left, bottom_right;
      //TODO: Calling get_visible_region here is probably the Wrong Thing to do
      get_visible_region(state->map, state->map_graphics_state->map_window_x_chars,
          state->map_graphics_state->map_window_y_chars, &top_left, &bottom_right);
      DEBUG("attempting to move cursor to %d, %d\n", target_point.x,
          target_point.y);
      DEBUG("top_left: (%d, %d)\n", top_left.x, top_left.y);
      DEBUG("bottom_right: (%d, %d)\n", bottom_right.x, bottom_right.y);
      if ( (target_point.x >= top_left.x)
          && (target_point.x <= bottom_right.x)
          && (target_point.y >= top_left.y)
          && (target_point.y <= bottom_right.y) ) {
        DEBUG("drawing cursor? maybe?\n");
        state->cursor_location = target_point;
        state->need_to_redraw_map = 1;
        // Get descriptor text
        Tile target_tile;
        target_tile = get_tile(state->map, target_point.x, target_point.y);
        const char* tile_desc;
        tile_desc = target_tile.type->description;
        DEBUG("got tile description %s\n", tile_desc);
        // draw look message
        DEBUG("Dest rect x: %d, y: %d, w: %d, h: %d\n",
            state->config->status_window.x,
            state->config->status_window.y,
            state->config->status_window.w,
            state->config->status_window.h);
        if ( render_look_message(tile_desc, &state->config->status_window) == -1) {
          exit(-1);
        }
        // flag status window to redraw
        state->need_to_redraw_status = 1;
      }
    }
  }
}

