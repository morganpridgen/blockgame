#ifndef stateh
#define stateh
#include <TEXEL/texel.h>
#include "player.h"
#include "level.h"

class GameState {
  public:
    virtual bool init() = 0;
    virtual GameState *update(TXL_Controller*[4]) = 0;
    virtual void render() = 0;
    virtual void end() = 0;
};

class PlayState : public GameState {
  private:
    Player ply;
    Level lvl;
  public:
    virtual bool init();
    virtual GameState *update(TXL_Controller*[4]);
    virtual void render();
    virtual void end();
};

class LevelSelectState : public GameState {
  private:
    char **levels;
    int levelCount;
    int selectedLevel;
    float lJX;
  public:
    virtual bool init();
    virtual GameState *update(TXL_Controller*[4]);
    virtual void render();
    virtual void end();
};

#endif