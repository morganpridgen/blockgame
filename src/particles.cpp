#include "particles.h"
#include <TEXEL/texel.h>

#define particleCount 128

void Particle::init(ParticleInfo &newInfo) {
  info = newInfo;
}

void Particle::update() {
  if (!getActive()) return;
  info.x += info.xV;
  info.y += info.yV;
  info.xV *= 0.9f;
  info.yV *= 0.9f;
  info.timer--;
}

void Particle::render() {
  if (getActive()) TXL_RenderQuad(info.x, info.y, info.s, info.s, {info.r, info.g, info.b, 1.0f});
}

Particle particles[particleCount];

void initParticles() {
  ParticleInfo newInfo = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 1.0f, 1.0f, 1.0f};
  for (int i = 0; i < particleCount; i++) particles[i].init(newInfo);
}

void addParticle(ParticleInfo &newInfo) {
  for (int i = 0; i < particleCount; i++) {
    if (!particles[i].getActive()) {
      particles[i].init(newInfo);
      break;
    }
  }
}

void updateParticles() {
  for (int i = 0; i < particleCount; i++) particles[i].update();
}

void renderParticles() {
  for (int i = 0; i < particleCount; i++) particles[i].render();
}