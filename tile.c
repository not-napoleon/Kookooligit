#include <stdlib.h>
#include <string.h>

#include <tile.h>

#define LOGGING_ENABLED
#include <log.h>


static bool _was_initilized = false;

bool tiles_initilized() {
  return _was_initilized;
}

TileType *_make_tile_type(const char *glyph, const char* description, bool is_passable) {
  TileType *result;
  result = malloc(sizeof(TileType));
  if (result == NULL) {
    CRITICAL("Couldn't malloc space for a tile type\n");
    exit(1);
  }
  result->glyph = strdup(glyph);
  result->description = strdup(description);
  if (is_passable == true) {
    result->is_passable = 1;
  } else {
    result->is_passable = 0;
  }
  return result;
}


void init_tile_types() {
  tile_data[OffGrid] = _make_tile_type(" ", "Unexplored space", true);
  tile_data[OpenSpace] = _make_tile_type(".", "An unobstructed corridor "
      "of some kind", true);
  tile_data[ImpassableWall] = _make_tile_type("#", "A hardened bulkhead, "
      "probably with hardened vaccuum on the other side", false);
  tile_data[RockWall] = _make_tile_type("#", "A rough stone wall, part "
      "of the asteroid <SHIPNAME> was carved from", false);
  tile_data[RockCorridor] = _make_tile_type("~", "A rough, natural passage "
      "through the asteroid", true);
  tile_data[TechWall] = _make_tile_type("%", "A a wall made of an unknown "
      "metal, by an unknown builder", false);
  tile_data[TechCorridor] = _make_tile_type(".", "A metal corridor. The "
      "height suggests the builder was not human", true);
  _was_initilized = true;
  INFO("Tile Data initilized\n");
}

void free_tile_types() {
  int i;
  for (i = 0; i < TILE_TYPE_COUNT; i++) {
    if (tile_data[i] != NULL) {
      free(tile_data[i]->description);
      free(tile_data[i]);
    }
  }
}
