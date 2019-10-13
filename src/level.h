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
    int gemCount, gemTarget;
    int eActiveFade;
    int eX, eY;
    float cX, cY;
  public:
    Level();
    bool init(const char*, Player&, int);
    bool update(Player&, TXL_Controller*);
    void render();
    void renderOverlay();
    void end();
    
    Tile *tileAt(int x, int y) {return tiles[y * lW + x];}
    int getW() {return lW;}
    int getH() {return lH;}
    float getCX() {return cX;}
    float getCY() {return cY;}
};

#endif