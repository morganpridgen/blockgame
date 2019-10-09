#include "tile.h"
#include <TEXEL/texel.h>
#include "player.h"

bool FloorTile::init() {
  return 1;
}

Tile *FloorTile::update(int tX, int tY, Player &ply) {
  return nullptr;
}

void FloorTile::render(int tX, int tY) {
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 16, 16, {0.25f, 0.25f, 0.25f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 14, 14, {0.5f, 0.5f, 0.5f, 1.0f});
}

void FloorTile::end() {

}



bool WallTile::init() {
  return 1;
}

Tile *WallTile::update(int tX, int tY, Player &ply) {
  return nullptr;
}

void WallTile::render(int tX, int tY) {
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 16, 16, {0.25f, 0.25f, 0.25f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 14, 14, {0.0f, 0.0f, 0.0f, 1.0f});
}

void WallTile::end() {

}



bool PortalTile::init() {
  pDest = -1;
  active = 0;
  return 1;
}

Tile *PortalTile::update(int tX, int tY, Player &ply) {
  int pX, pY;
  ply.getPos(pX, pY);
  if (tX == pX && tY == pY) active = 1;
  return nullptr;
}

void PortalTile::render(int tX, int tY) {
  FloorTile::render(tX, tY);
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 8, 8, {1.0f, 0.0f, 1.0f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 6, 6, {0.5f, 0.0f, 0.5f, 1.0f});
}

void PortalTile::end() {

}



bool GemTile::init() {
  return 1;
}

Tile *GemTile::update(int tX, int tY, Player &ply) {
  int pX, pY;
  ply.getPos(pX, pY);
  if (tX == pX && tY == pY) return new FloorTile;
  return nullptr;
}

void GemTile::render(int tX, int tY) {
  FloorTile::render(tX, tY);
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 8, 8, {1.0f, 0.0f, 0.0f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 6, 6, {0.5f, 0.0f, 0.0f, 1.0f});
}

void GemTile::end() {

}



Tile *getTileId(int id) {
  Tile *out = nullptr;
  switch (id) {
    case 0:
      out = new FloorTile;
      break;
    case 1:
      out = new WallTile;
      break;
    case 2:
      out = new PortalTile;
      break;
    case 3:
      out = new GemTile;
      break;
  }
  return out;
}