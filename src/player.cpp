#include "player.h"
#include <TEXEL/texel.h>
#include "particles.h"

bool Player::init() {
  x = 1, y = 1;
  mTU = 0, mTD = 0, mTL = 0, mTR = 0;
  dead = 0;
  respawnTimer = 120;
  if (!tex.load(TXL_DataPath("player.png"), 16, 16)) return 0;
  return 1;
}

void Player::update(TXL_Controller &ctrl, Level &lvl) {
  if (dead) {
    respawnTimer--;
    if (respawnTimer == 60) {
      for (int i = 0; i < 16; i++) {
        ParticleInfo pInfo = {x * 16 + 8, y * 16 + 8, 2.0f * cos(float(i) / (8.0f / 3.14f)), 2.0f * sin(float(i) / (8.0f / 3.14f)), 2.0f, 60, 1.0f, 1.0f, 1.0f};
        addParticle(pInfo);
      }
    }
    return;
  }
  
  lX = x, lY = y;
  if (ctrl.leftJoyX() >= 0.5f && !lvl.tileAt(x + 1, y)->isSolid()) {
    if (mTR == 0) {
      x += 1;
      mTR = 15;
    } else mTR--;
  } else mTR = 0;
  if (ctrl.leftJoyX() <= -0.5f && !lvl.tileAt(x - 1, y)->isSolid()) {
    if (mTL == 0) {
      x -= 1;
      mTL = 15;
    } else mTL--;
  } else mTL = 0;
  if (ctrl.leftJoyY() >= 0.5f && !lvl.tileAt(x, y + 1)->isSolid()) {
    if (mTD == 0) {
      y += 1;
      mTD = 15;
    } else mTD--;
  } else mTD = 0;
  if (ctrl.leftJoyY() <= -0.5f && !lvl.tileAt(x, y - 1)->isSolid()) {
    if (mTU == 0) {
      y -= 1;
      mTU = 15;
    } else mTU--;
  } else mTU = 0;
  if (x < 0) x = 0;
  if (x >= lvl.getW()) x = lvl.getW() - 1;
  if (y < 0) y = 0;
  if (y >= lvl.getH()) y = lvl.getH() - 1;
  
  float tVX = x * 16 + 8, tVY = y * 16 + 8;
  if (vXV < (tVX - vX) / 2.0f) vXV++;
  if (vXV > (tVX - vX) / 2.0f) vXV--;
  if (vYV < (tVY - vY) / 2.0f) vYV++;
  if (vYV > (tVY - vY) / 2.0f) vYV--;
  vX += vXV;
  vY += vYV;
}

void Player::render(float cX, float cY) {
  if (respawnTimer < 60) return;
  /*TXL_RenderQuad(x * 16 + 8 + cX, y * 16 + 8 + cY, 8, 8, {0.5f * float(dead), 0.5f * float(!dead), 0.0f, 1.0f});
  TXL_RenderQuad(x * 16 + 8 + cX, y * 16 + 8 + cY, 6, 6, {1.0f * float(dead), 1.0f * float(!dead), 0.0f, 1.0f});*/
  //tex.render(x * 16 + 8 + cX, y * 16 + 8 + cY);
  tex.render(vX + cX, vY + cY, 270.0f * float(dead));
}

void Player::end() {
  tex.free();
}