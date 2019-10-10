#include "tile.h"
#include <TEXEL/texel.h>
#include "player.h"

bool FloorTile::init() {
  return 1;
}

Tile *FloorTile::update(int tX, int tY, Player &ply, Level &lvl) {
  tSolid = lvl.tileAt(tX, tY - 1)->isSolid();
  lSolid = lvl.tileAt(tX - 1, tY)->isSolid();
  tLSolid = lvl.tileAt(tX - 1, tY - 1)->isSolid();
  return nullptr;
}

void FloorTile::render(int tX, int tY) {
  floorTex.render(tX * 16 + 8, tY * 16 + 8);
  if (tSolid) TXL_RenderQuad({tX * 16 + 4, tY * 16, 12, 4}, {0.0f, 0.0f, 0.0f, 0.5f});
  if (lSolid) TXL_RenderQuad({tX * 16, tY * 16 + 4, 4, 12}, {0.0f, 0.0f, 0.0f, 0.5f});
  if (tLSolid) TXL_RenderQuad({tX * 16, tY * 16, 4, 4}, {0.0f, 0.0f, 0.0f, 0.5f});
}

void FloorTile::end() {
}



bool WallTile::init() {
  return 1;
}

Tile *WallTile::update(int tX, int tY, Player &ply, Level &lvl) {
  return nullptr;
}

void WallTile::render(int tX, int tY) {
  /*TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 16, 16, {0.25f, 0.25f, 0.25f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 14, 14, {0.0f, 0.0f, 0.0f, 1.0f});*/
  wallTex.render(tX * 16 + 8, tY * 16 + 8);
}

void WallTile::end() {
}



bool PortalTile::init() {
  animTimer = 0;
  pDest = -1;
  active = 0;
  return 1;
}

Tile *PortalTile::update(int tX, int tY, Player &ply, Level &lvl) {
  FloorTile::update(tX, tY, ply, lvl);
  int pX, pY;
  ply.getPos(pX, pY);
  if (tX == pX && tY == pY) {
    TXL_Noise teleport = {1.0f, 2.0f, 0, 8, 0};
    TXL_PlaySound(teleport);
    active = 1;
  }
  return nullptr;
}

void PortalTile::render(int tX, int tY) {
  FloorTile::render(tX, tY);
  /*TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 8, 8, {1.0f, 0.0f, 1.0f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 6, 6, {0.5f, 0.0f, 0.5f, 1.0f});*/
  int offset = ((animTimer / 8) + tX + tY) % 4;
  portalTex.setClip(offset * 16, (offset + 1) * 16, 0, 16);
  portalTex.setColorMod(0.95f);
  portalTex.render(tX * 16 + 8, tY * 16 + 8, ((animTimer / 4) % 4) * 90);
  animTimer++;
}

void PortalTile::end() {

}



bool GemTile::init() {
  animTimer = 0;
  return 1;
}

Tile *GemTile::update(int tX, int tY, Player &ply, Level &lvl) {
  FloorTile::update(tX, tY, ply, lvl);
  int pX, pY;
  ply.getPos(pX, pY);
  if (tX == pX && tY == pY) {
    TXL_Square pling = {3520, 1.0f, 2.0f, 1};
    TXL_PlaySound(pling);
    return new FloorTile;
  }
  return nullptr;
}

void GemTile::render(int tX, int tY) {
  FloorTile::render(tX, tY);
  /*TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 8, 8, {1.0f, 0.0f, 0.0f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 6, 6, {0.5f, 0.0f, 0.0f, 1.0f});*/
  int offset = (((animTimer / 8) + tX + tY) % 8 < 4) ? 0 : ((animTimer / 8) + tX + tY) % 4;
  gemTex.setClip(offset * 16, (offset + 1) * 16, 0, 16);
  gemTex.setColorMod(0.95f);
  gemTex.render(tX * 16 + 8, tY * 16 + 8);
  animTimer++;
}

void GemTile::end() {

}



bool BoxTile::init() {
  bDir = -1;
  return 1;
}

Tile *BoxTile::update(int tX, int tY, Player &ply, Level &lvl) {
  FloorTile::update(tX, tY, ply, lvl);
  int pX, pY;
  ply.getPos(pX, pY);
  if (tX == pX && tY == pY) {
    int pLX, pLY;
    ply.getLPos(pLX, pLY);
    if (pX != pLX && pY != pLY) pX = pLX, pY = pLY;
    else if (lvl.tileAt(pX + (pX - pLX), pY + (pY - pLY))->getId() != 0) pX = pLX, pY = pLY;
    else {
      if (pX - pLX == 1) bDir = 0;
      if (pX - pLX == -1) bDir = 2;
      if (pY - pLY == 1) bDir = 1;
      if (pY - pLY == -1) bDir = 3;
      TXL_Noise push = {1.0f, 3.0f, 0, 16, 1};
      TXL_PlaySound(push);
    }
    ply.setPos(pX, pY);
  }
  return bDir == -1 ? nullptr : new FloorTile;
}

void BoxTile::render(int tX, int tY) {
  FloorTile::render(tX, tY);
  /*TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 16, 16, {0.25f, 0.125f, 0.0f, 1.0f});
  TXL_RenderQuad(tX * 16 + 8, tY * 16 + 8, 14, 14, {0.5f, 0.25f, 0.0f, 1.0f});*/
  boxTex.render(tX * 16 + 8, tY * 16 + 8);
}

void BoxTile::end() {

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
    case 4:
      out = new BoxTile;
      break;
  }
  return out;
}