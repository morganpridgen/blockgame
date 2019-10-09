#include "level.h"
#include <cstring>
#include <TEXEL/texel.h>
#include "tile.h"
#include "player.h"

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
}

bool Level::init(const char *name, Player &ply, int lNum) {
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

void Level::update(Player &ply) {
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
      if (!newTile->init()) continue;
      tiles[i]->end();
      delete tiles[i];
      tiles[i] = newTile;
    }
    if (tiles[i]->portalActive() != -1) portalActive = tiles[i]->portalActive();
  }
  if (portalActive != -1) {
    end();
    init(levelName, ply, portalActive);
  }
}

void Level::render() {
  for (int i = 0; i < lW * lH; i++) tiles[i]->render(i % lW, i / lW);
}

void Level::end() {
  for (int i = 0; i < lW * lH; i++) {
    tiles[i]->end();
    delete tiles[i];
  }
  delete [] tiles;
  tiles = nullptr;
}