#ifndef SNOWBALL_H
#define SNOWBALL_H

#include <list>

#include "voxspr.h"
#include "voximg.h"
#include "vox_scene.h"

//does it have to be a Vox_Particle?
class Snowball : public VoxSpr
{
public:
    Snowball(Pt3d pos, Pt3d velocity);
    virtual ~Snowball();
    void move();
    bool to_destroy;//when has to be destroyed
    static Vox_Scene* ground;
    static void update_snowballs();//update all
    static void blit_all(Vox_Scene *scene);
    static VoxImg snowballimage;
private:
    Pt3d m_velocity;
    static std::list<Snowball*> snowballs;
};

#endif // VOX_PARTICLE_H
