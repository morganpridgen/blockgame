#ifndef tileh
#define tileh

#include "player.h"

class Tile {
  public:
    virtual bool init() {return 1;}
    virtual void update(int tX, int tY, Player &ply) {}
    virtual void render(int tX, int tY) {}
    virtual void end() {}
    
    virtual int getId() {return -1;}
    virtual bool isSolid() {return 0;}
};

class FloorTile : public Tile {
  public:
    virtual bool init();
    virtual void update(int, int, Player&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 0;}
};

#endif