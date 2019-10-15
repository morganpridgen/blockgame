#include "entities.h"
#include <cmath>
#include <TEXEL/texel.h>
#include "level.h"
#include "player.h"

#define arrowCount 16
Arrow arrows[arrowCount];
TXL_Texture arrowTex;

Arrow::Arrow() {
  info.x = 0, info.y = 0, info.r = 0;
  active = 1;
}

void Arrow::update(Level &lvl, Player &ply) {
  info.x += 2.0f * cos(info.r);
  info.y += 2.0f * sin(info.r);
  if (lvl.tileAt(info.x / 16, info.y / 16)->isSolid()) active = 0; 
  
  if (ply.isDead()) return;
  int pTX, pTY;
  float pX, pY;
  ply.getPos(pTX, pTY);
  pX = pTX * 16 + 8, pY = pTY * 16 + 8;
  if ((pX - info.x) * (pX - info.x) + (pY - info.y) * (pY - info.y) < 144) {
    active = 0;
    ply.kill();
  }
}

void Arrow::render(TXL_Texture &tex, float cX, float cY) {
  tex.setClip(48, 64, 0, 16);
  tex.render(info.x + cX, info.y + cY, info.r * (180.0f / 3.14f));
}

bool initArrows() {
  for (int i = 0; i < arrowCount; i++) arrows[i].setActive(0);
  if (!arrowTex.load(TXL_DataPath("crossbow.png"), 64, 16)) return 0;
  return 1;
}

void addArrow(ArrowInfo &info) {
  for (int i = 0; i < arrowCount; i++) {
    if (!arrows[i].getActive()) {
      arrows[i].setActive(1);
      arrows[i].setInfo(info);
      return;
    }
  }
}

void updateArrows(Level &lvl, Player &ply) {
  for (int i = 0; i < arrowCount; i++) {
    if (arrows[i].getActive()) arrows[i].update(lvl, ply);
  }
}

void renderArrows(float cX, float cY) {
  for (int i = 0; i < arrowCount; i++) {
    if (arrows[i].getActive()) arrows[i].render(arrowTex, cX, cY);
  }
}

void endArrows() {

}