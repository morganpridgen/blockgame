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



bool LevelSelectState::init() {
  TXL_File f;
  if (!f.init(TXL_DataPath("levels/list.txt"), 'r')) return 0;
  char tmpLevels[64][64];
  for (int i = 0; i < 64; i++) memset(tmpLevels[i], 0, sizeof(tmpLevels[i]));
  int lvlNum = 0, lvlLetter = 0;
  char tmp;
  while (1) {
    if (!f.read(&tmp, sizeof(tmp))) break;
    if (tmp <= ' ') {
      lvlNum++;
      lvlLetter = 0;
    } else {
      tmpLevels[lvlNum][lvlLetter] = tmp;
      lvlLetter++;
    }
  }
  f.close();
  levelCount = lvlNum + 1;
  levels = new char*[levelCount];
  for (int i = 0; i < levelCount; i++) {
    levels[i] = new char[strlen(tmpLevels[i])];
    memcpy(levels[i], tmpLevels[i], sizeof(levels[i]));
  }
  return 1;
}

GameState *LevelSelectState::update(TXL_Controller *ctrls[4]) {
  return nullptr;
}

void LevelSelectState::render() {
  for (int i = 0; i < levelCount; i++) {
    TXL_Texture *lvl = TXL_RenderText(levels[i], 1.0f, 1.0f, 1.0f);
    lvl->render(320.0f, 180.0f + 16.0f * i);
    delete lvl;
  }
}

void LevelSelectState::end() {
  for (int i = 0; i < levelCount; i++) delete [] levels[i];
  delete [] levels;
  levels = nullptr;
}