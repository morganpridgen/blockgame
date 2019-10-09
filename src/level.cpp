#include "level.h"
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

bool Level::init(const char *name, Player &ply, int lNum) {
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
      tiles[i] = getTileId(id);
      if (!tiles || !tiles[i]->init()) return 0;
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
    Tile *newTile = tiles[i]->update(i % lW, i / lW, ply);
    if (newTile) {
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