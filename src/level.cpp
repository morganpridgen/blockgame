#include "level.h"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <TEXEL/texel.h>
#include "tile.h"
#include "player.h"
#include "particles.h"

TXL_Texture floorTex, wallTex, portalTex, gemTex, boxTex;
TXL_Texture exitTex;

int nextInt(TXL_File *f) {
  int out = 0;
  char tmp = 0;
  bool readNum = 0;
  while (1) {
    if (!f->read(&tmp, sizeof(tmp))) {
      if (readNum) return out;
      else return -1;
    }
    if (tmp >= '0' && tmp <= '9') readNum = 1;
    else if (readNum) return out;
    if (readNum) {
      out *= 10;
      out += tmp - '0';
    }
  }
}

Level::Level() {
  tiles = nullptr;
  lW = 0, lH = 0, room = -1;
  memset(levelName, 0, sizeof(levelName));
  memset(gems, 0, sizeof(gems));
  for (int i = 0; i < 64; i++) gems[i].room = -1;
  gemCount = 0;
  gemTarget = 0;
  eActiveFade = 0;
}

bool Level::init(const char *name, Player &ply, int lNum) {
  if (!floorTex.load(TXL_DataPath("floor.png"), 16, 16)) return 0;
  if (!wallTex.load(TXL_DataPath("wall.png"), 16, 16)) return 0;
  if (!portalTex.load(TXL_DataPath("portal.png"), 64, 16)) return 0;
  if (!gemTex.load(TXL_DataPath("gem.png"), 64, 16)) return 0;
  if (!boxTex.load(TXL_DataPath("box.png"), 16, 16)) return 0;
  if (!exitTex.load(TXL_DataPath("exit.png"), 48, 48)) return 0;

  eX = -1, eY = -1;
  room = lNum;
  strcpy(levelName, name);
  if (!name) {
    lW = 16, lH = 16;
    tiles = new Tile*[lW * lH];
    for (int j = 0; j < lW; j++) {
      for (int i = 0; i < lH; i++) {
        if (j >= 4 && j <= 11 && i == 12) tiles[i * lW + j] = new WallTile;
        else tiles[i * lW + j] = new FloorTile;
        if (!tiles[i * lW + j]->init()) return 0;
      }
    }
    ply.setPos(2, 2);
  } else {
    TXL_File f;
    char path[256];
    sprintf(path, "%s/%s/targetGems.txt", TXL_DataPath("levels"), name);
    if (!f.init(path, 'r')) return 0;
    gemTarget = nextInt(&f);
    f.close();
    sprintf(path, "%s/%s/%i.txt", TXL_DataPath("levels"), name, lNum);
    if (!f.init(path, 'r')) return 0;
    lW = nextInt(&f), lH = nextInt(&f);
    int pX = nextInt(&f), pY = nextInt(&f);
    ply.setPos(pX, pY);
    tiles = new Tile*[lW * lH];
    int portals[8];
    memset(portals, -1, sizeof(portals));
    for (int i = 0; i < lW * lH; i++) {
      int id = nextInt(&f);
      if (id == 2) {
        for (int j = 0; j < 8; j++) {
          if (portals[j] == -1) {
            portals[j] = i;
            break;
          }
        }
      }
      if (id == 3) {
        for (int j = 0; j < 64; j++) {
          if (gems[j].room == lNum && gems[j].pos == i) {
            id = 0;
            break;
          }
        }
      }
      if (id == 5) {
        eX = i % lW, eY = i / lW;
        id = 0;
      }
      tiles[i] = getTileId(id);
      if (!tiles[i] || !tiles[i]->init()) return 0;
    }
    for (int i = 0; i < 8; i++) {
      if (portals[i] != -1) {
        tiles[portals[i]]->setPortalDest(nextInt(&f));
      }
    }
    f.close();
  }
  return 1;
}

void Level::update(Player &ply, TXL_Controller *ctrl) {
  int portalActive = -1;
  for (int i = 0; i < lW * lH; i++) {
    Tile *newTile = tiles[i]->update(i % lW, i / lW, ply, *this);
    if (newTile) {
      if (tiles[i]->isGem()) {
        for (int j = 0; j < 64; j++) {
          if (gems[j].room == -1) {
            gems[j].room = room;
            gems[j].pos = i;
            break;
          }
        }
        gemCount++;
        ctrl->rumble(0.5, 250);
      }
      if (tiles[i]->boxDir() != -1) {
        int nX = i % lW, nY = i / lW;
        if (tiles[i]->boxDir() == 0) nX += 1;
        if (tiles[i]->boxDir() == 1) nY += 1;
        if (tiles[i]->boxDir() == 2) nX -= 1;
        if (tiles[i]->boxDir() == 3) nY -= 1;
        Tile *newBox = new BoxTile;
        if (!newBox->init()) continue;
        delete tiles[nY * lW + nX];
        tiles[nY * lW + nX] = newBox;
      }
      if (!newTile->init()) {
        delete newTile;
        continue;
      }
      tiles[i]->end();
      delete tiles[i];
      tiles[i] = newTile;
    }
    if (tiles[i]->portalActive() != -1) portalActive = tiles[i]->portalActive();
  }
  if (gemCount >= gemTarget && eActiveFade < 64) eActiveFade++;
  if (eX > -1 && eY > -1) {
    for (int i = 0; i < 4 * (float(gemCount) / float(gemTarget)); i++) {
      float d = float(rand()) / (float(RAND_MAX) / 24.0f), r = float(rand()) / (float(RAND_MAX) / 6.28f);
      ParticleInfo info = {eX * 16 + 8 + d * cos(r), eY * 16 + 8 + d * sin(r), 0, 0, 2.0f, 15, 1.0f, 0.0f, 1.0f};
      addParticle(info);
    }
  }
  
  if (portalActive != -1) {
    end();
    init(levelName, ply, portalActive);
  }
}

void Level::render() {
  for (int i = 0; i < lW * lH; i++) tiles[i]->render(i % lW, i / lW);
}

void Level::renderOverlay() {
  if (eActiveFade) TXL_RenderQuad({0, 0, lW * 16, lH * 16}, {1.0f, 0.0f, 1.0f, float(eActiveFade) / 1024.0f});
  if (eX > -1 && eY > -1) {
    exitTex.setColorMod(2.0f / 8.0f);
    for (int i = 0; i < 8 * fmin((float(gemCount * gemCount) / float(gemTarget * gemTarget)), 1.0f); i++) exitTex.render(eX * 16 + 6 + rand() % 4, eY * 16 + 6 + rand() % 4);
  }
  char labelText[32];
  sprintf(labelText, ": %i/%i", gemCount, gemTarget);
  TXL_Texture *label = TXL_RenderText(labelText, 1.0f, 1.0f, 1.0f * (gemCount < gemTarget));
  label->render(48 + label->width() / 2, 16 + label->height() / 2 + 16 * lH);
  label->free();
  delete label;
  gemTex.setColorMod(1.0f);
  gemTex.setClip(0, 16, 0, 16);
  gemTex.render(48 - gemTex.width() / 2, 16 + gemTex.height() / 2 + 16 * lH, 2.0f, 2.0f);
}

void Level::end() {
  floorTex.free();
  wallTex.free();
  portalTex.free();
  gemTex.free();
  boxTex.free();
  for (int i = 0; i < lW * lH; i++) {
    tiles[i]->end();
    delete tiles[i];
  }
  delete [] tiles;
  tiles = nullptr;
}