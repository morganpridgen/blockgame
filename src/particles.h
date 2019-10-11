#ifndef particlesh
#define particlesh

struct ParticleInfo {
  float x, y, xV, yV, s;
  int timer;
  float r, g, b;
};

class Particle {
  private:
    ParticleInfo info;
  public:
    void init(ParticleInfo&);
    void update();
    void render(float, float);
    
    bool getActive() {return info.timer > 0;}
};

void initParticles();
void addParticle(ParticleInfo&);
void updateParticles();
void renderParticles(float, float);

#endif