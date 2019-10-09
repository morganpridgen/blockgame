#include "player.h"
#include <TEXEL/texel.h>

bool Player::init() {
  x = 1, y = 1;
  lJX = 0, lJY = 0;
}

void Player::update(TXL_Controller &ctrl) {
  if (lJX < 64 && ctrl.leftJoyX() >= 64) x += 1;
  if (lJX > -64 && ctrl.leftJoyX() <= -64) x -= 1;
  if (lJY < 64 && ctrl.leftJoyY() >= 64) y += 1;
  if (lJY > -64 && ctrl.leftJoyY() <= -64) y -= 1;
  lJX = ctrl.leftJoyX(), lJY = ctrl.leftJoyY();
}

void Player::render() {
  TXL_RenderQuad(x * 16 + 8, y * 16 + 8, 8, 8, {0.0f, 0.5f, 0.0f, 1.0f});
  TXL_RenderQuad(x * 16 + 8, y * 16 + 8, 6, 6, {0.0f, 1.0f, 0.0f, 1.0f});
}

void Player::end() {

}