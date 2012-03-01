#ifndef VOX_SCENE_H
#define VOX_SCENE_H

#include <string>

#include "voximg.h"
#include "voxspr.h"

//snow indexes of snow voxels, from SNOW_START to SNOW_END-1
#define SNOW_START 28
#define SNOW_END   32


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
class Particle_Manager;

class Vox_Scene : public VoxImg
{
public:
    Vox_Scene(int _xsiz, int _ysiz, int _zsiz, VoxImg & palette_model);//get palette from this VoxImg
    virtual ~Vox_Scene();
    void init();
    void blit(VoxSpr & obj);
    void blit(VoxImg * obj,Pt3d * pos);
    int sub(VoxSpr & obj,bool throw_particle=true,unsigned short color_start=0xFF,unsigned short color_end=0xFF);//remove non-empty obj voxels from the scene (remove only one color if color != 0xFF), return number of voxels destroyed
    void add(VoxSpr & obj,bool replace=true,bool throw_doubles=true);//add non-empty obj voxels to the scene (throw a particle if not replace and voxel already taken)
    bool collide(VoxSpr & obj);//voxel-perfect collision
    bool touch_bottom(VoxSpr & obj);
    bool touch_top(VoxSpr & obj);
    bool touch_center(VoxSpr & obj);
    bool reinit(VoxImg & obj);//must be the same size !
};

#endif // VOX_SCENE_H
