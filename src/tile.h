#ifndef tileh
#define tileh

class Tile;

#include "player.h"
#include "level.h"

class Tile {
  private:
  public:
    virtual bool init() {return 1;}
    virtual Tile *update(int tX, int tY, Player &ply, Level &lvl) {return nullptr;}
    virtual void render(int tX, int tY) {}
    virtual void end() {}
    
    virtual int getId() {return -1;}
    virtual bool isSolid() {return 0;}
    virtual int portalActive() {return -1;}
    virtual void setPortalDest(int dest) {;}
    virtual bool isGem() {return 0;}
    virtual int boxDir() {return -1;}
};

class FloorTile : public Tile {
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 0;}
};

class WallTile : public Tile {
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 1;}
    virtual bool isSolid() {return 1;}
};

class PortalTile : public FloorTile {
  private:
    int pDest;
    bool active;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 2;}
    virtual int portalActive() {return active ? pDest : -1;}
    virtual void setPortalDest(int dest) {pDest = dest;}
};

class GemTile : public FloorTile {
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 3;}
    virtual bool isGem() {return 1;}
};

class BoxTile : public Tile {
  private:
    int bDir;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int);
    virtual void end();
    
    virtual int getId() {return 4;}
    virtual int boxDir() {return bDir;}
};

Tile *getTileId(int);

#endif