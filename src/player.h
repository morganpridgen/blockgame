#ifndef playerh
#define playerh

class Player;

#include <TEXEL/texel.h>
#include "level.h"

class Player {
  private:
    int x, y, lX, lY;
    float lJX, lJY;
    int mTU, mTD, mTL, mTR;
  public:
    bool init();
    void update(TXL_Controller&, Level&);
    void render();
    void end();
    
    void getPos(int &nX, int &nY) {nX = x, nY = y;}
    void setPos(int nX, int nY) {x = nX, y = nY;}
    void getLPos(int &nX, int &nY) {nX = lX, nY = lY;}
};

#endif