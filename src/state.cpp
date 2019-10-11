#include "state.h"
#include <TEXEL/texel.h>

bool PlayState::init() {
  if (!ply.init()) return 0;
  if (!lvl.init("test", ply, 0)) return 0;
  return 1;
}

GameState *PlayState::update(TXL_Controller *ctrls[4]) {
  ply.update(*ctrls[0], lvl);
  lvl.update(ply, ctrls[0]);
  return nullptr;
}

void PlayState::render() {
  lvl.render();
  ply.render(lvl.getCX(), lvl.getCY());
  lvl.renderOverlay();
}

void PlayState::end() {
  lvl.end();
  ply.end();
}