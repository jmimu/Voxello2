#include "vox_scene.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "particle_manager.h"

//get palette and size from this VoxImg
Vox_Scene::Vox_Scene(VoxImg & model)
{
    xsiz=model.get_xsiz();
    ysiz=model.get_ysiz();
    zsiz=model.get_zsiz();

    xyzsiz=xsiz*ysiz*zsiz;
    yzsiz=ysiz*zsiz;

#ifdef VOX_24BIT
    voxels = (unsigned int *)malloc(xyzsiz*sizeof(unsigned int));
#else
    voxels = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));
#endif
    for (int i=0;i<255;i++)
    {
        palette[i][0]=model.palette[i][0];
        palette[i][1]=model.palette[i][1];
        palette[i][2]=model.palette[i][2];
    }
}


//get palette from this VoxSpr
Vox_Scene::Vox_Scene(int _xsiz, int _ysiz, int _zsiz, VoxImg & palette_model) : VoxImg()
{
    xsiz=_xsiz;
    ysiz=_ysiz;
    zsiz=_zsiz;

    xyzsiz=xsiz*ysiz*zsiz;
    yzsiz=ysiz*zsiz;

#ifdef VOX_24BIT
    voxels = (unsigned int *)malloc(xyzsiz*sizeof(unsigned int));
#else
    voxels = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));
#endif
    for (int i=0;i<255;i++)
    {
        palette[i][0]=palette_model.palette[i][0];
        palette[i][1]=palette_model.palette[i][1];
        palette[i][2]=palette_model.palette[i][2];
    }
}

Vox_Scene::~Vox_Scene()
{
    if ((Particle_Manager::the_one) and (Particle_Manager::the_one->get_scene()==this))
        delete Particle_Manager::the_one;
}

void Vox_Scene::init()
{
    long z_start;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
        {
            z_start=cos(x/40.0)*5+sin(y/24.0+1)*8+12;
            if (z_start<=0) z_start=1; 
            for (long z=0;z<zsiz;z++)
            {
                if (z<z_start)
                    voxels[x*yzsiz+y*zsiz+(zsiz-z-1)]=z_start/2;//0x50;
                else if (z<z_start+1)//for snow
                    voxels[x*yzsiz+y*zsiz+(zsiz-z-1)]=rand()%(SNOW_END-SNOW_START)+SNOW_START;//0x50;
                else
                    voxels[x*yzsiz+y*zsiz+(zsiz-z-1)]=0xFF;
            }
        }
    //add a wall
    
    //colors: 20 and 150
    //structure:
    //   ***************
    //   *   *   *   *  
    //   *   *   *   *  
    //   ***************
    //     *   *   *   *
    //     *   *   *   *
    unsigned char v=0;
    for (long x=65;x<68;x++)
    {
        for (long y=6;y<30;y++)
        {
            for (long z=0;z<zsiz-10;z++)
            {
                v=150;
                if (z%3==0) v=20;
                if ((y+3*((z/3)%2))%5==0) v=20;
                voxels[x*yzsiz+y*zsiz+(zsiz-z-1)]=v;
            }
        }
    }
    
    //other
    v=0;
    for (long x=100;x<102;x++)
    {
        for (long y=80;y<82;y++)
        {
            for (long z=0;z<zsiz;z++)
            {
                v=201;
                if (z%2==0) v=78;
                voxels[x*yzsiz+y*zsiz+z]=v;
            }
        }
    }
    
    
}

void Vox_Scene::blit(VoxSpr & spr)
{
    blit(spr.get_img(),spr.get_pos());
}


void Vox_Scene::blit(VoxImg * obj,Pt3d *pos)
{
    long obj_i,this_i;
    int pos_x=pos->x-obj->get_xsiz()/2;
    int pos_y=pos->y-obj->get_ysiz()/2;
    int pos_z=pos->z-obj->get_zsiz();
    obj_i=0;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z;
    //std::cout<<"\nstart at "<<pos_x<<" "<<pos_y<<" "<<pos_z<<": "<<this_i<<"\n";
    for (long x=0;x<obj->get_xsiz();x++)
    {
        //std::cout<<"debug: "<<(x+pos_x)<<" "<<this_i<<std::endl;
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            for (long y=0;y<obj->get_ysiz();y++)
            {
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    for (long z=0;z<obj->get_zsiz();z++)
                    {
                        if (!(((z+pos_z)<0)||((z+pos_z)>=zsiz)))
                        {
                            //obj_i=x*obj->get_yzsiz()+y*obj->get_zsiz()+z;
                            if (obj->voxels[obj_i]!=0xFF)
                            {
                                //this_i=(x+pos_x)*yzsiz+(y+pos_y)*zsiz+(z+pos_z);
                                voxels[this_i]=obj->voxels[obj_i];
                            }
                        }
                        obj_i++;
                        this_i++;
                    }
                    this_i+=(zsiz-obj->get_zsiz());
                }else{obj_i+=obj->get_zsiz();this_i+=zsiz;}
            }
            this_i+=(ysiz-obj->get_ysiz())*(zsiz);
        }else{/*std::cout<<"AAAr"<<std::endl;*/obj_i+=obj->get_yzsiz();this_i+=yzsiz;}
    }
    //std::cout<<"Finish\n\n"<<std::endl;
}


bool Vox_Scene::touch_bottom(VoxSpr & obj)
{
    //std::cout<<"touch_bottom "<<obj.get_pos()->x<<" "<<obj.get_pos()->y<<" "<<obj.get_pos()->z<<std::endl;
    long obj_i,this_i;
    int pos_x=obj.get_pos()->x-obj.get_xsiz()/2;
    int pos_y=obj.get_pos()->y-obj.get_ysiz()/2;
    int pos_z=obj.get_pos()->z-obj.get_zsiz();
    obj_i=obj.get_zsiz()-1;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z+obj.get_zsiz();
    if ((pos_z+obj.get_zsiz()<0)||(pos_z+obj.get_zsiz()>=zsiz-1)) return false;
    //std::cout<<"   "<<this_i<<"   "<<obj_i<<std::endl;
    for (long x=0;x<obj.get_xsiz();x++)
    {
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            //std::cout<<" a  "<<this_i<<"   "<<obj_i<<"   "<<x<<" "<<pos_x<<" "<<std::endl;
            for (long y=0;y<obj.get_ysiz();y++)
            {
                //std::cout<<" b  "<<this_i<<"   "<<obj_i<<"   "<<x<<" "<<pos_x<<" "<<y<<" "<<pos_y<<std::endl;
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    //std::cout<<" c  "<<this_i<<"   "<<obj_i<<"   "<<x<<" "<<pos_x<<" "<<y<<" "<<pos_y<<std::endl;
                    if (voxels[this_i]!=0xFF)
                    {
                        if (obj.get_img()->voxels[obj_i]!=0xFF)
                            return true;
                    }
                }
                obj_i+=obj.get_zsiz();this_i+=zsiz;
            }
        }else{obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
        this_i+=(ysiz-obj.get_ysiz())*(zsiz);
    }
    return false;
}


bool Vox_Scene::touch_top(VoxSpr & obj)
{
    long obj_i,this_i;
    int pos_x=obj.get_pos()->x-obj.get_xsiz()/2;
    int pos_y=obj.get_pos()->y-obj.get_ysiz()/2;
    int pos_z=obj.get_pos()->z-obj.get_zsiz();
    obj_i=0;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z-1;
    if ((pos_z<1)||(pos_z>=zsiz)) return false;
    for (long x=0;x<obj.get_xsiz();x++)
    {
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            for (long y=0;y<obj.get_ysiz();y++)
            {
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    if (voxels[this_i]!=0xFF)
                    {
                        if (obj.get_img()->voxels[obj_i]!=0xFF)
                            return true;
                    }
                }
                obj_i+=obj.get_zsiz();this_i+=zsiz;
            }
        }else{obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
        this_i+=(ysiz-obj.get_ysiz())*(zsiz);
    }
    return false;
}

//voxel-perfect collision
bool Vox_Scene::collide(VoxSpr & obj)
{
    long obj_i,this_i;
    int pos_x=obj.get_pos()->x-obj.get_xsiz()/2;
    int pos_y=obj.get_pos()->y-obj.get_ysiz()/2;
    int pos_z=obj.get_pos()->z-obj.get_zsiz();
    obj_i=0;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z;
    for (long x=0;x<obj.get_xsiz();x++)
    {
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            for (long y=0;y<obj.get_ysiz();y++)
            {
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    for (long z=0;z<obj.get_zsiz();z++)
                    {
                        if (!(((z+pos_z)<0)||((z+pos_z)>=zsiz)))
                        {
                            //obj_i=x*obj.get_yzsiz()+y*obj.get_zsiz()+z;
                            //this_i=(x+pos_x)*yzsiz+(y+pos_y)*zsiz+(z+pos_z);
                            if ((voxels[this_i]!=0xFF)&&(obj.get_img()->voxels[obj_i]!=0xFF))
                                 return true;
                        }
                        obj_i++;
                        this_i++;
                    }
                    this_i+=(zsiz-obj.get_zsiz());
                }else{obj_i+=obj.get_zsiz();this_i+=zsiz;}
            }
            this_i+=(ysiz-obj.get_ysiz())*(zsiz);
        }else{obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
    }
    return false;
}


bool Vox_Scene::touch_center(VoxSpr & obj)
{
    long this_i;
    int pos_x=obj.get_pos()->x;
    int pos_y=obj.get_pos()->y;
    int pos_z=obj.get_pos()->z-obj.get_zsiz()/2;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z;
    if ((this_i>=0)&&(this_i<xyzsiz))
        if (voxels[this_i]!=0xFF)
            return true;
    return false;
}


//remove non-empty obj voxels from the scene, return number of voxels destroyed
int Vox_Scene::sub(VoxSpr & obj,bool throw_particle,unsigned short color_start,unsigned short color_end)
{
    long obj_i,this_i;
    long nbr_vox_sub=0;
    int pos_x=obj.get_pos()->x-obj.get_xsiz()/2;
    int pos_y=obj.get_pos()->y-obj.get_ysiz()/2;
    int pos_z=obj.get_pos()->z-obj.get_zsiz();
    Pt3d particle_pos;
    obj_i=0;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z;
    for (long x=0;x<obj.get_xsiz();x++)
    {
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            for (long y=0;y<obj.get_ysiz();y++)
            {
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    for (long z=0;z<obj.get_zsiz();z++)
                    {
                        if (!(((z+pos_z)<0)||((z+pos_z)>=zsiz)))
                        {
                            //obj_i=x*obj.get_yzsiz()+y*obj.get_zsiz()+z;
                            if (obj.get_img()->voxels[obj_i]!=0xFF)
                            {
                                //this_i=(x+pos_x)*yzsiz+(y+pos_y)*zsiz+(z+pos_z);
                                if ((voxels[this_i]!=0xFF)&&((color_start==0xFF)||((voxels[this_i]>=color_start)&&(voxels[this_i]<color_end))))
                                {
                                    if (throw_particle)
                                    {
                                        particle_pos.x=pos_x+x;
                                        particle_pos.y=pos_y+y;
                                        particle_pos.z=pos_z+z;
                                        Particle_Manager::the_one->add_particle(voxels[this_i],particle_pos);
                                    }
                                    voxels[this_i]=0xFF;
                                    nbr_vox_sub++;
                                }
                            }
                        }
/*                        obj_i++;
                        this_i++;
                    }
                }else{obj_i+=obj.get_zsiz();this_i+=zsiz;}
                this_i+=(zsiz-obj.get_zsiz());
            }
        }else{obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
        this_i+=(ysiz-obj.get_ysiz())*(zsiz);
    }*/
                        obj_i++;
                        this_i++;
                    }
                    this_i+=(zsiz-obj.get_zsiz());
                }else{obj_i+=obj.get_zsiz();this_i+=zsiz;}
            }
            this_i+=(ysiz-obj.get_ysiz())*(zsiz);
        }else{/*std::cout<<"AAAr"<<std::endl;*/obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
    }

    return nbr_vox_sub;
}


//add non-empty obj voxels to the scene
void Vox_Scene::add(VoxSpr & obj,bool replace,bool throw_doubles)
{
    long obj_i,this_i;
    int pos_x=obj.get_pos()->x-obj.get_xsiz()/2;
    int pos_y=obj.get_pos()->y-obj.get_ysiz()/2;
    int pos_z=obj.get_pos()->z-obj.get_zsiz();
    Pt3d particle_pos;
    obj_i=0;
    this_i=pos_x*yzsiz+pos_y*zsiz+pos_z;
    for (long x=0;x<obj.get_xsiz();x++)
    {
        if (!(((x+pos_x)<0)||((x+pos_x)>=xsiz)))
        {
            for (long y=0;y<obj.get_ysiz();y++)
            {
                if (!(((y+pos_y)<0)||((y+pos_y)>=ysiz)))
                {
                    for (long z=0;z<obj.get_zsiz();z++)
                    {
                        if (!(((z+pos_z)<0)||((z+pos_z)>=zsiz)))
                        {
                            //obj_i=x*obj.get_yzsiz()+y*obj.get_zsiz()+z;
                            if (obj.get_img()->voxels[obj_i]!=0xFF)
                            {
                                if (replace)
                                {
                                    voxels[this_i]=obj.get_img()->voxels[obj_i];
                                }
                                else
                                {
                                    if(voxels[this_i]==0xFF)
                                        voxels[this_i]=obj.get_img()->voxels[obj_i];
                                    else if (throw_doubles)
                                    {
                                        particle_pos.x=pos_x+x;
                                        particle_pos.y=pos_y+y;
                                        particle_pos.z=pos_z+z;
                                        Particle_Manager::the_one->add_particle(obj.get_img()->voxels[obj_i],particle_pos);
                                    }
                                }
                            }
                        }
                        obj_i++;
                        this_i++;
                    }
                }else{obj_i+=obj.get_zsiz();this_i+=zsiz;}
                this_i+=(zsiz-obj.get_zsiz());
            }
        }else{obj_i+=obj.get_yzsiz();this_i+=yzsiz;}
        this_i+=(ysiz-obj.get_ysiz())*(zsiz);
    }
}


//must be the same size !
bool Vox_Scene::reinit(VoxImg & obj)
{
    if ((xsiz!=obj.get_xsiz())||(ysiz!=obj.get_ysiz())||(zsiz!=obj.get_zsiz()))
    {
        std::cout<<"Error, not same voxel size in Vox_Scene::reinit\n";
        return false;
    }

#ifdef VOX_24BIT
    unsigned int * vox_src=obj.voxels;
    unsigned int * vox_dest=voxels;
#else
    unsigned char * vox_src=obj.voxels;
    unsigned char * vox_dest=voxels;
#endif


    for (long i=0;i<xyzsiz;i++)
    {
        (*vox_dest)=(*vox_src);
        vox_src++;
        vox_dest++;
    }
    
    
    return true;
}

