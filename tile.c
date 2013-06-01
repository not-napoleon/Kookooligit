#include <tile.h>

const static char* off_grid_desc = "Unexplored space";
const static char* open_space_desc = "An unobstructed corridor of some kind";
const static char* impassable_wall_desc = "A hardened bulkhead, probably with hardened vaccuum on the other side";

const char* get_tile_description(TileType tt) {
  /*
   * TODO: When we get config & data files working, this will all get read out
   * of a data file.  Hard code for now.
   */
  switch(tt) {
    case OffGrid:
      return off_grid_desc;
    case OpenSpace:
      return open_space_desc;
    case ImpassableWall:
      return impassable_wall_desc;
  };
}
