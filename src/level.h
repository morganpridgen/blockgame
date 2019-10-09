#ifndef levelh
#define levelh

#include "tile.h"
#include "player.h"

class Level {
  private:
    Tile **tiles;
    int lW, lH;
  public:
    bool init(char*, Player&);
    void update(Player&);
    void render();
    void end();
};

#endif