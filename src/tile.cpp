#include "tile.h"
#include <TEXEL/texel.h>
#include "player.h"

bool FloorTile::init() {
  return 1;
}

void FloorTile::update(int tX, int tY, Player &ply) {

}

void FloorTile::render(int tX, int tY) {
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 16, 16, {0.25f, 0.25f, 0.25f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 14, 14, {0.5f, 0.5f, 0.5f, 1.0f});
}

void FloorTile::end() {

}