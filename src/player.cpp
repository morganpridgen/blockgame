#include "player.h"
#include <TEXEL/texel.h>

bool Player::init() {
  x = 1, y = 1;
  lJX = 0, lJY = 0;
  mTU = 0, mTD = 0, mTL = 0, mTR = 0;
}

void Player::update(TXL_Controller &ctrl, Level &lvl) {
  lX = x, lY = y;
  if (ctrl.leftJoyX() >= 64 && !lvl.tileAt(x + 1, y)->isSolid()) {
    if (mTR == 0) {
      x += 1;
      mTR = 15;
    } else mTR--;
  } else mTR = 0;
  if (ctrl.leftJoyX() <= -64 && !lvl.tileAt(x - 1, y)->isSolid()) {
    if (mTL == 0) {
      x -= 1;
      mTL = 15;
    } else mTL--;
  } else mTL = 0;
  if (ctrl.leftJoyY() >= 64 && !lvl.tileAt(x, y + 1)->isSolid()) {
    if (mTD == 0) {
      y += 1;
      mTD = 15;
    } else mTD--;
  } else mTD = 0;
  if (ctrl.leftJoyY() <= -64 && !lvl.tileAt(x, y - 1)->isSolid()) {
    if (mTU == 0) {
      y -= 1;
      mTU = 15;
    } else mTU--;
  } else mTU = 0;
  if (x < 0) x = 0;
  if (x >= lvl.getW()) x = lvl.getW() - 1;
  if (y < 0) y = 0;
  if (y >= lvl.getH()) y = lvl.getH() - 1;
  lJX = ctrl.leftJoyX(), lJY = ctrl.leftJoyY();
}

void Player::render() {
  TXL_RenderQuad(x * 16 + 8, y * 16 + 8, 8, 8, {0.0f, 0.5f, 0.0f, 1.0f});
  TXL_RenderQuad(x * 16 + 8, y * 16 + 8, 6, 6, {0.0f, 1.0f, 0.0f, 1.0f});
}

void Player::end() {

}