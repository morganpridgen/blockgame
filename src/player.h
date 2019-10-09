#ifndef playerh
#define playerh
#include <TEXEL/texel.h>

class Player {
  private:
    int x, y;
    float lJX, lJY;
  public:
    bool init();
    void update(TXL_Controller& );
    void render();
    void end();
    
    void getPos(int &nX, int &nY) {nX = x, nY = y;}
    void setPos(int nX, int nY) {x = nX, y = nY;}
};

#endif