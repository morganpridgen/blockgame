#include "state.h"
#include <TEXEL/texel.h>

char playingLevel[64];

bool PlayState::init() {
  if (!ply.init()) return 0;
  if (!lvl.init(playingLevel, ply, 0)) return 0;
  return 1;
}

GameState *PlayState::update(TXL_Controller *ctrls[4]) {
  ply.update(*ctrls[0], lvl);
  if (lvl.update(ply, ctrls[0])) return new WinState;
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
  selectedLevel = 0, lJX = 0.0f;
  return 1;
}

GameState *LevelSelectState::update(TXL_Controller *ctrls[4]) {
  if (ctrls[0]->leftJoyX() > 0.5f && lJX <= 0.5f && selectedLevel != levelCount - 1) selectedLevel++; 
  if (ctrls[0]->leftJoyX() < -0.5f && lJX >= -0.5f && selectedLevel != 0) selectedLevel--;
  if (ctrls[0]->buttonClick(CtrlA)) {
    strcpy(playingLevel, levels[selectedLevel]);
    return new PlayState;
  }
  lJX = ctrls[0]->leftJoyX();
  return nullptr;
}

void LevelSelectState::render() {
  TXL_Texture *label = TXL_RenderText(levels[selectedLevel], 1.0f, 1.0f, 1.0f);
  label->render(320.0f, 180.0f);
  delete label;
}

void LevelSelectState::end() {
  for (int i = 0; i < levelCount; i++) delete [] (levels[i]);
  delete [] levels;
  levels = nullptr;
}



bool WinState::init() {
  return 1;
}

GameState *WinState::update(TXL_Controller *ctrls[4]) {
  if (ctrls[0]->buttonClick(CtrlA)) return new LevelSelectState;
  return nullptr;
}

void WinState::render() {
  char text[64];
  sprintf(text, "You beat %s!", playingLevel);
  TXL_Texture *label = TXL_RenderText(text, 1.0f, 1.0f, 1.0f);
  label->render(320.0f, 180.0f);
  delete label;
}

void WinState::end() {

}