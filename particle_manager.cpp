#include "particle_manager.h"

#include <time.h>
#include <iostream>

Particle_Manager * Particle_Manager::the_one=NULL;

Particle_Manager::Particle_Manager(Vox_Scene *scene,Vox_Scene *ground): m_scene(scene),m_ground(ground),m_particles()
{
    srand(time(NULL));
    the_one=this;
}

Particle_Manager::~Particle_Manager()
{
    for(std::list<Vox_Particle*>::iterator it =  m_particles.begin(); it != m_particles.end(); ++it)
    {
        delete *it;
    }
    m_particles.clear();
}

void Particle_Manager::blit_all()
{
    for(std::list<Vox_Particle*>::iterator it =  m_particles.begin(); it != m_particles.end(); ++it)
    {
        m_scene->blit(**it);
    }
}

void Particle_Manager::update()
{
    //std::cout<<"Particle_Manager::update() "<<m_particles.size()<<std::endl;
    for(std::list<Vox_Particle*>::iterator it =  m_particles.begin(); it != m_particles.end(); ++it)
    {
        (*it)->move();
        if (((*it)->get_pos()->z)>=m_ground->zsiz) (*it)->to_destroy=true;
        if ((((*it)->get_pos()->x)<0)||(((*it)->get_pos()->x)>=m_ground->xsiz)) (*it)->to_destroy=true;
        if ((((*it)->get_pos()->y)<0)||(((*it)->get_pos()->y)>=m_ground->ysiz)) (*it)->to_destroy=true;
        if ((m_ground->touch_bottom(**it)))
        {
            (*it)->to_destroy=true;
            m_ground->add(**it,false,false);
        }
        if ((*it)->to_destroy)
        {
            delete *it;
            it=m_particles.erase(it);
        }
    }
}

void Particle_Manager::add_particle(unsigned char color, Pt3d pos)
{
    Pt3d velocity( (rand()%(200)-100)/400.0,(rand()%(200)-100)/400.0,-(rand()%(200)+100)/200.0);
    add_particle(color,pos,velocity,true);
}

void Particle_Manager::add_particle(unsigned char color, Pt3d pos, Pt3d velocity, bool acceleration)
{
    m_particles.push_back(new Vox_Particle(color,pos,velocity,acceleration));
}

