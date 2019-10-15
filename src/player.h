#ifndef playerh
#define playerh

class Player;

#include <TEXEL/texel.h>
#include "level.h"

class Player {
  private:
    int x, y, lX, lY;
    int mTU, mTD, mTL, mTR;
    bool dead;
    int respawnTimer;
  public:
    bool init();
    void update(TXL_Controller&, Level&);
    void render(float, float);
    void end();
    
    void getPos(int &nX, int &nY) {nX = x, nY = y;}
    void setPos(int nX, int nY) {x = nX, y = nY;}
    void getLPos(int &nX, int &nY) {nX = lX, nY = lY;}
    bool isDead() {return dead;}
    void kill() {dead = 1;}
    void revive() {dead = 0, respawnTimer = 300;}
    bool respawn() {return !respawnTimer;}
};

#endif