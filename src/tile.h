#ifndef tileh
#define tileh

class Tile;

#include <TEXEL/texel.h>
#include "player.h"
#include "level.h"

extern TXL_Texture floorTex, wallTex, portalTex, gemTex, boxTex, bowTex;

class Tile {
  protected:
    int animTimer;
  public:
    virtual bool init() {return 1;}
    virtual Tile *update(int tX, int tY, Player &ply, Level &lvl) {return nullptr;}
    virtual void render(int tX, int tY, float cX, float cY) {}
    virtual void end() {}
    
    virtual int getId() {return -1;}
    virtual bool isSolid() {return 0;}
    virtual int portalActive() {return -1;}
    virtual void setPortalDest(int dest) {;}
    virtual bool isGem() {return 0;}
    virtual int boxDir() {return -1;}
    virtual void gemColor(int nR1, int nG1, int nB1, int nR2, int nG2, int nB2) {}
};

class FloorTile : public Tile {
  protected:
    bool tSolid, lSolid, tLSolid;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int, float, float);
    virtual void end();
    
    virtual int getId() {return 0;}
};

class WallTile : public Tile {
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int, float, float);
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
    virtual void render(int, int, float, float);
    virtual void end();
    
    virtual int getId() {return 2;}
    virtual int portalActive() {return active ? pDest : -1;}
    virtual void setPortalDest(int dest) {pDest = dest;}
};

class GemTile : public FloorTile {
  private:
    int r1, g1, b1, r2, g2, b2;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int, float, float);
    virtual void end();
    
    virtual int getId() {return 3;}
    virtual bool isGem() {return 1;}
    virtual void gemColor(int nR1, int nG1, int nB1, int nR2, int nG2, int nB2) {r1 = nR1, g1 = nG1, b1 = nB1, r2 = nR2, g2 = nG2, b2 = nB2;}
};

class BoxTile : public FloorTile {
  private:
    int bDir;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int, float, float);
    virtual void end();
    
    virtual int getId() {return 4;}
    virtual int boxDir() {return bDir;}
};

class CrossbowTile : public FloorTile {
  private:
    float r, tR;
    int shotTimer;
  public:
    virtual bool init();
    virtual Tile *update(int, int, Player&, Level&);
    virtual void render(int, int, float, float);
    virtual void end();
    
    virtual int getId() {return 7;}
    virtual bool isSolid() {return 1;}
};

Tile *getTileId(int);

#endif