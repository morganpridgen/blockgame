#ifndef levelh
#define levelh

class Level;

#include "tile.h"
#include "player.h"

class Level {
  private:
    struct Gem {
      int room;
      int pos;
    };
    Tile **tiles;
    int lW, lH, room;
    char levelName[32];
    Gem gems[64];
  public:
    Level();
    bool init(const char*, Player&, int);
    void update(Player&);
    void render();
    void end();
    
    Tile *tileAt(int x, int y) {return tiles[y * lW + x];}
    int getW() {return lW;}
    int getH() {return lH;}
};

#endif