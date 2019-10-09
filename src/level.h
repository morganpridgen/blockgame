#ifndef levelh
#define levelh

class Level;

#include "tile.h"
#include "player.h"

class Level {
  private:
    Tile **tiles;
    int lW, lH;
    char levelName[32];
  public:
    bool init(const char*, Player&, int);
    void update(Player&);
    void render();
    void end();
    
    Tile *tileAt(int x, int y) {return tiles[y * lW + x];}
    int getW() {return lW;}
    int getH() {return lH;}
};

#endif