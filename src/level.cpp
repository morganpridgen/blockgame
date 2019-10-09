#include "level.h"
#include <TEXEL/texel.h>
#include "tile.h"
#include "player.h"

bool Level::init(char *name, Player &ply) {
  if (!name) {
    lW = 16, lH = 16;
    tiles = new Tile*[lW * lH];
    for (int j = 0; j < lW; j++) {
      for (int i = 0; i < lH; i++) {
        tiles[i * lW + j] = new FloorTile;
        if (!tiles[i * lW + j]->init()) return 0;
      }
    }
    ply.setPos(2, 2);
  }
  return 1;
}

void Level::update(Player &ply) {
  for (int i = 0; i < lW * lH; i++) tiles[i]->update(i % lW, i / lW, ply);
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