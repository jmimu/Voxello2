#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <string>
#include <list>
#include <cstdlib>

#include "voxspr.h"
#include "vox_scene.h"
#include "vox_particle.h"


/**
  Object's frame of reference

      / North (+y)
     /
    /
   /_ _ _ _ East (+x)
  |
  |
  |
  Down (+z)
  */

class Particle_Manager
{
public:
    Particle_Manager(Vox_Scene *scene,Vox_Scene *ground);
    virtual ~Particle_Manager();
    void update();
    void add_particle(unsigned char color, Pt3d pos);
    void add_particle(unsigned char color, Pt3d pos, Pt3d velocity, bool acceleration=true);
    void blit_all();
    
    static Particle_Manager * the_one;
    Vox_Scene *get_scene(){return m_scene;};
private:
    Vox_Scene *m_scene;
    Vox_Scene *m_ground;
    std::list <Vox_Particle*> m_particles;
};

#endif // PARTICLE_MANAGER_H
