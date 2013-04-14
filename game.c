#include <game.h>

void process_command(GameState *state, CommandCode cmd) {
  printf("Processing Command %d - state is %d\n", cmd, state->state);
  if (cmd == NoOp) {
    return;
  }
  Point target_point;
  if (state->state == Move) {
    target_point = state->at_location;
  } else if (state->state == Look) {
    target_point = state->cursor_location;
  }

  switch (cmd) {
    case EnterLookMode:
      printf("Enter look mode\n");
      if (state->state == Move) {
        state->state = Look;
        state->cursor_location.x = state->at_location.x;
        state->cursor_location.y = state->at_location.y;
      }
      break;
    case ExitLookMode:
      printf("Exiting look mode\n");
      if (state->state == Look) {
        state->state = Move;
        state->cursor_location.x = state->at_location.x;
        state->cursor_location.y = state->at_location.y;
      }
      break;
    case MoveLeft:
      target_point.x -= 1;
      printf("Move left\n");
      break;
    case MoveRight:
      target_point.x += 1;
      printf("Move right\n");
      break;
    case MoveUp:
      target_point.y -= 1;
      printf("Move up\n");
      break;
    case MoveDown:
      target_point.y += 1;
      printf("Move down\n");
      break;
    case MoveUpLeft:
      target_point.x -= 1;
      target_point.y -= 1;
      printf("Move up left\n");
      break;
    case MoveDownLeft:
      target_point.x -= 1;
      target_point.y += 1;
      printf("Move down left\n");
      break;
    case MoveUpRight:
      target_point.x += 1;
      target_point.y -= 1;
      printf("Move up right\n");
      break;
    case MoveDownRight:
      target_point.x += 1;
      target_point.y += 1;
      printf("Move down right\n");
      break;
    case Quit:
      state->is_running = 0;
      break;
  }
  if (state->state == Move) {
    if (is_passable_point(state->map, target_point) == 1) {
      state->at_location = target_point;
      state->need_to_redraw_map = 1;
    } else {
      char tmp[50] = "You can't walk through walls";
      add_message(state->messages, tmp, state->font);
      state->need_to_redraw_messages = 1;
    }
    // Decide if we need to recenter the map section
    if (abs(state->at_location.x - state->map->center.x) > 10) {
      state->map->center.x = state->at_location.x;
    }
    if (abs(state->at_location.y - state->map->center.y) > 10) {
      state->map->center.y = state->at_location.y;
    }
    printf("at_location (%i, %i), center (%i, %i)\n", state->at_location.x,
        state->at_location.y, state->map->center.x, state->map->center.y);
    calculate_visible_tiles(state->map, state->at_location);
  } else if (state->state == Look) {
    Point top_left, bottom_right;
    get_visible_region(state->map, state->map_window_x_chars,
        state->map_window_y_chars, &top_left, &bottom_right);
    printf("attempting to move cursor to %d, %d\n", target_point.x,
        target_point.y);
    printf("top_left: (%d, %d)\n", top_left.x, top_left.y);
    printf("bottom_right: (%d, %d)\n", bottom_right.x, bottom_right.y);
    if ( (target_point.x >= top_left.x)
        && (target_point.x <= bottom_right.x)
        && (target_point.y >= top_left.y)
        && (target_point.y <= bottom_right.y) ) {
      printf("drawing cursor? maybe?\n");
      state->cursor_location = target_point;
      state->need_to_redraw_map = 1;
    }
  }
}

