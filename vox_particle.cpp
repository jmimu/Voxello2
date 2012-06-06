#include "vox_particle.h"

#include <stdlib.h>
#include <iostream>

Vox_Particle::Vox_Particle(unsigned char color, Pt3d pos, Pt3d velocity,bool acceleration)
    : to_destroy(false),m_acceleration(acceleration),m_velocity(velocity)
{
    m_img=new VoxImg();
    m_img->xsiz=1;
    m_img->ysiz=1;
    m_img->zsiz=1;
#ifdef VOX_24BIT
    m_img->voxels = (unsigned int *)malloc(sizeof(unsigned int));
#else
    m_img->voxels = (unsigned char *)malloc(sizeof(unsigned char));
#endif
    m_img->voxels[0]=color;
    set_pos(pos);
    //std::cout<<"New Particle: "<<pos.x<<" "<<pos.y<<" "<<pos.z<<": "<<m_velocity.z<<std::endl;

#ifdef VOX_24BIT
    m_img->voxels[0]=(((unsigned int)color)<<16)+(((unsigned int)color)<<8)+(((unsigned int)color)<<0);
#endif
}


Vox_Particle::~Vox_Particle()
{
    //std::cout<<"Delete particle: "<<pos.x<<" "<<pos.y<<" "<<pos.z<<": "<<m_velocity.z<<std::endl;
    delete m_img;
}


void Vox_Particle::move()
{
    m_pos.x+=m_velocity.x;
    m_pos.y+=m_velocity.y;
    m_pos.z+=m_velocity.z;
    if (m_acceleration) m_velocity.z+=0.1;
}
