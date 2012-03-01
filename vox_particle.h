#ifndef VOX_PARTICLE_H
#define VOX_PARTICLE_H

#include <string>

#include "voxspr.h"
#include "voximg.h"


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


class Vox_Particle : public VoxSpr
{
public:
    Vox_Particle(unsigned char color, Pt3d pos, Pt3d velocity,bool acceleration=true);
    virtual ~Vox_Particle();
    void move();
    bool to_destroy;//when has to be destroyed by the particle manager
private:
    bool m_acceleration;
    Pt3d m_velocity;
};

#endif // VOX_PARTICLE_H
