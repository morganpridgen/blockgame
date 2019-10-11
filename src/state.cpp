#include "state.h"
#include <TEXEL/texel.h>
#include "particles.h"

bool PlayState::init() {
  if (!ply.init()) return 0;
  if (!lvl.init("test", ply, 0)) return 0;
  initParticles();
  return 1;
}

GameState *PlayState::update(TXL_Controller *ctrls[4]) {
  ply.update(*ctrls[0], lvl);
  lvl.update(ply, ctrls[0]);
  updateParticles();
  return nullptr;
}

void PlayState::render() {
  /*for (int i = 0; i < 640 / 16; i++) {
    for (int j = 0; j < 360 / 16; j++) {
      TXL_RenderQuad({i * 16, j * 16, 16, 16}, {1.0f, 1.0f, 1.0f, 1.0f});
      TXL_RenderQuad({i * 16 + 1, j * 16 + 1, 14, 14}, {0.5f, 0.5f, 0.5f, 1.0f});
    }
  }*/
  lvl.render();
  ply.render();
  renderParticles();
  lvl.renderOverlay();
}

void PlayState::end() {
  lvl.end();
  ply.end();
}