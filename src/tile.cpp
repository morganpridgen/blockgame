#include "tile.h"
#include <cstdlib>
#include <cmath>
#include <TEXEL/texel.h>
#include "player.h"
#include "particles.h"

bool FloorTile::init() {
  return 1;
}

Tile *FloorTile::update(int tX, int tY, Player &ply, Level &lvl) {
  tSolid = lvl.tileAt(tX, tY - 1)->isSolid();
  lSolid = lvl.tileAt(tX - 1, tY)->isSolid();
  tLSolid = lvl.tileAt(tX - 1, tY - 1)->isSolid();
  return nullptr;
}

void FloorTile::render(int tX, int tY, float cX, float cY) {
  floorTex.render(tX * 16 + 8 + cX, tY * 16 + 8 + cY);
  if (tSolid) TXL_RenderQuad({tX * 16 + 4 + cX, tY * 16 + cY, 12, 4}, {0.0f, 0.0f, 0.0f, 0.5f});
  if (lSolid) TXL_RenderQuad({tX * 16 + cX, tY * 16 + 4 + cY, 4, 12}, {0.0f, 0.0f, 0.0f, 0.5f});
  if (tLSolid) TXL_RenderQuad({tX * 16 + cX, tY * 16 + cY, 4, 4}, {0.0f, 0.0f, 0.0f, 0.5f});
}

void FloorTile::end() {
}



bool WallTile::init() {
  return 1;
}

Tile *WallTile::update(int tX, int tY, Player &ply, Level &lvl) {
  return nullptr;
}

void WallTile::render(int tX, int tY, float cX, float cY) {
  wallTex.render(tX * 16 + 8 + cX, tY * 16 + 8 + cY);
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
  if (animTimer % 4 == 0) {
    ParticleInfo info = {tX * 16 + (rand() % 16), tY * 16 + (rand() % 16), 0, 0, 1.0f, 30, 1.0f, 0.0f, 1.0f};
    addParticle(info);
  }
  return nullptr;
}

void PortalTile::render(int tX, int tY, float cX, float cY) {
  FloorTile::render(tX, tY, cX, cY);
  int offset = ((animTimer / 8) + tX + tY) % 4;
  portalTex.setClip(offset * 16, (offset + 1) * 16, 0, 16);
  portalTex.setColorMod(0.95f);
  portalTex.render(tX * 16 + 8 + cX, tY * 16 + 8 + cY, ((animTimer / 4) % 4) * 90);
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
  if (animTimer % 8 == 0 && ((animTimer / 8) + tX + tY) % 8 == 5) {
    ParticleInfo info = {tX * 16 + 4, tY * 16 + 4, 0, 0, 1, 15, 0.75f, 0.5f, 0.0f};
    for (int i = 0; i < 8; i++) {
      info.xV = cos(float(i) * (6.28f / 8.0f));
      info.yV = sin(float(i) * (6.28f / 8.0f));
      addParticle(info);
    }
  }
  return nullptr;
}

void GemTile::render(int tX, int tY, float cX, float cY) {
  FloorTile::render(tX, tY, cX, cY);
  int offset = (((animTimer / 8) + tX + tY) % 8 < 4) ? 0 : ((animTimer / 8) + tX + tY) % 4;
  gemTex.setClip(offset * 16, (offset + 1) * 16, 0, 16);
  gemTex.setColorMod(0.95f);
  gemTex.render(tX * 16 + 8 + cX, tY * 16 + 8 + cY);
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

void BoxTile::render(int tX, int tY, float cX, float cY) {
  FloorTile::render(tX, tY, cX, cY);
  boxTex.render(tX * 16 + 8 + cX, tY * 16 + 8 + cY);
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