#include "snowball.h"

#include <stdlib.h>
#include <iostream>

VoxImg Snowball::snowballimage("data/snowball2.vox",0,VOX_FILE);
Vox_Scene * Snowball::ground=NULL;
std::list<Snowball*> Snowball::snowballs;

Snowball::Snowball(Pt3d pos, Pt3d velocity)
    : to_destroy(false),m_velocity(velocity)
{
    if (Snowball::ground==NULL) std::cout<<"Error! Snowball::ground not initialized!"<<std::endl;
    set_img(&snowballimage);
    set_pos(pos);
    Snowball::snowballs.push_back(this);
}


Snowball::~Snowball()
{
}


void Snowball::move()
{
    m_pos.x+=m_velocity.x;
    m_pos.y+=m_velocity.y;
    m_pos.z+=m_velocity.z;
    m_velocity.z+=0.1;
}

void Snowball::update_snowballs()
{
    for(std::list<Snowball*>::iterator it =  Snowball::snowballs.begin(); it != Snowball::snowballs.end(); ++it)
    {
        (*it)->move();
        if (((*it)->get_pos()->z)>=ground->zsiz) (*it)->to_destroy=true;
        if ((((*it)->get_pos()->x)<0)||(((*it)->get_pos()->x)>=ground->xsiz)) (*it)->to_destroy=true;
        if ((((*it)->get_pos()->y)<0)||(((*it)->get_pos()->y)>=ground->ysiz)) (*it)->to_destroy=true;
        if ((ground->collide(**it)))//touch_center is too dangerous (snowballs are fast)
        {
            (*it)->to_destroy=true;
            ground->add(**it,false,true);
        }
        if ((*it)->to_destroy)
        {
            delete *it;
            it=Snowball::snowballs.erase(it);
        }
    }
}


void Snowball::blit_all(Vox_Scene *scene)
{
    for(std::list<Snowball*>::iterator it =  snowballs.begin(); it != snowballs.end(); ++it)
    {
        scene->blit(**it);
    }
}


