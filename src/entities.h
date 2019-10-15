#ifndef entitiesh
#define entitiesh

class Level;
class Player;

#include <TEXEL/texel.h>
#include "level.h"
#include "player.h"

struct ArrowInfo {
  float x, y, r;
};

class Arrow {
  private:
    ArrowInfo info;
    bool active;
  public:
    Arrow();
    void update(Level&, Player&);
    void render(TXL_Texture&, float, float);
    bool getActive() {return active;}
    void setActive(bool a) {active = a;}
    void setInfo(ArrowInfo &newInfo) {info = newInfo;}
};
bool initArrows();
void addArrow(ArrowInfo&);
void updateArrows(Level&, Player&);
void renderArrows(float, float);
void endArrows();

#endif