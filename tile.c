#include <tile.h>

const char* get_tile_description(TileType tt) {
  /*
   * TODO: When we get config & data files working, this will all get read out
   * of a data file.  Hard code for now.
   */
  switch(tt) {
    case OffGrid:
      return "Unexplored space";
    case OpenSpace:
      return "An unobstructed corridor of some kind.";
    case ImpassableWall:
      return "A hardened bulkhead, probably with hardened vaccuum on the other side";
  };
}
