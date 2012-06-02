#include "voximg.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "ply_io/ply_io.h"


VoxImg::VoxImg() :
        voxels(0),
        xsiz(0), ysiz(0), zsiz(0)
{
    //std::cout<<"Empty VoxImg created."<<std::endl;
}

VoxImg::VoxImg(std::string filnam,int direction,int file_format) :
        voxels(0),
        xsiz(0), ysiz(0), zsiz(0)
{
    //DESKLAMP  dopefish  duke  globe  pawn  strongbad CHAIR1
    //load_from_VOX("data/duke.vox");
    switch (file_format)
    {
    case VOX_FILE:
        load_from_VOX(filnam,direction);
        break;
    default:
        std::cout<<"Bad file format: "<<filnam<<" format "<<file_format<<std::endl;
    }

    std::cout<<"VoxImg created."<<std::endl;
}


VoxImg::~VoxImg()
{
    if (voxels)
        free(voxels);
}

///load Ken Silverman's VOX format
//todo: bad to load the whole file in one time
bool VoxImg::load_from_VOX (std::string filnam,int direction)
{
    FILE *fil;
    
    //unsigned char *voxels;


    fil = fopen(filnam.c_str(),"rb"); if (!fil) return(false);
    fread(&xsiz,4,1,fil); //size
    fread(&ysiz,4,1,fil); //size
    fread(&zsiz,4,1,fil); //size

    unsigned short xsiz_tmp, ysiz_tmp, zsiz_tmp;//before rotation
    xsiz_tmp=xsiz;
    ysiz_tmp=ysiz;
    zsiz_tmp=zsiz;

    std::cout<<filnam<<": got "<<xsiz_tmp<<"*"<<ysiz_tmp<<"*"<<zsiz_tmp<<" voxels_tmp"<<std::endl;

    unsigned char *voxels_tmp = (unsigned char *)malloc(xsiz_tmp*ysiz_tmp*zsiz_tmp*sizeof(unsigned char));//before rotation
    voxels = (unsigned char *)malloc(xsiz_tmp*ysiz_tmp*zsiz_tmp*sizeof(unsigned char));
    if (!voxels_tmp)
        std::cout<<"Error voxels_tmp"<<std::endl;
    if (!voxels)
        std::cout<<"Error voxels"<<std::endl;

    fread(voxels_tmp,xsiz_tmp*ysiz_tmp*zsiz_tmp,1,fil); //The 3-D array itself!

    fread(palette,768,1,fil);          //VGA palette (values range from 0-63)
    fclose(fil);
    //colour 255 is for empty space

    //change palette form 18 to 24 bits
    for (int i=0;i<255;i++)
    {
        palette[i][0]=palette[i][0]<<2;
        palette[i][1]=palette[i][1]<<2;
        palette[i][2]=palette[i][2]<<2;
    }
    //just for tests: change black (inside) to yellow
    //palette[0][0]=200;    palette[0][1]=200;    palette[0][2]=00;

    //rotation
    long x,y,z;

    if ((direction==EAST)||(direction==WEST))
    {
        xsiz=xsiz_tmp;
        ysiz=ysiz_tmp;
    }else{
        xsiz=ysiz_tmp;
        ysiz=xsiz_tmp;
    }
    zsiz=zsiz_tmp;

    std::cout<<"direction "<<direction<<std::endl;
    x=0;y=0;z=0;
    for (int x_tmp=0;x_tmp<xsiz_tmp;x_tmp++)
    {
        for (int y_tmp=0;y_tmp<ysiz_tmp;y_tmp++)
        {
            for (int z_tmp=0;z_tmp<zsiz_tmp;z_tmp++)
            {
                //std::cout<<(int)voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp]<<" ";
                if (direction==EAST)
                    voxels[x*ysiz*zsiz+y*zsiz+z]=voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp];
                if (direction==WEST)
                    voxels[(xsiz-x-1)*ysiz*zsiz+y*zsiz+z]=voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp];
                if (direction==NORTH)
                    voxels[x*ysiz*zsiz+y*zsiz+z]=voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp];
                if (direction==SOUTH)
                    voxels[x*ysiz*zsiz+(ysiz-y-1)*zsiz+z]=voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp];
                z++;
            }
            if ((direction==EAST)||(direction==WEST))
                y++;
            else
                x++;
            z=0;
        }
        if ((direction==EAST)||(direction==WEST))
        {
            x++;
            y=0;
        }else{
            y++;
            x=0;
        }
    }



/*    long x,y,z;
    if (direction==EAST)
    {
        std::cout<<"East direction"<<std::endl;
        xsiz=xsiz_tmp;
        ysiz=ysiz_tmp;
        zsiz=zsiz_tmp;

        x=0;y=0;z=0;
        for (int x_tmp=0;x_tmp<xsiz_tmp;x_tmp++)
        {
            for (int y_tmp=0;y_tmp<ysiz_tmp;y_tmp++)
            {
                for (int z_tmp=0;z_tmp<zsiz_tmp;z_tmp++)
                {
                    voxels[x*ysiz*zsiz+y*zsiz+z]=voxels_tmp[x_tmp*ysiz_tmp*zsiz_tmp+y_tmp*zsiz_tmp+z_tmp];
                    z++;
                }
                y++;
                z=0;
            }
            x++;
            y=0;
        }
    }*/

    free(voxels_tmp);

    xyzsiz=xsiz*ysiz*zsiz;
    yzsiz=ysiz*zsiz;

    std::cout<<filnam<<": got "<<xsiz<<"*"<<ysiz<<"*"<<zsiz<<" voxels"<<std::endl;

    long nbr_vox=0;

    unsigned char v;
    for (long x=0;x<xsiz;x++)
        for (long y=0;y<ysiz;y++)
            for (long z=0;z<zsiz;z++)
            {
                v=voxels[x*yzsiz+y*zsiz+z];
                if (v!=255)
                {
                    //std::cout<<"Try to add voxel "<<x<<" "<<y<<" "<<z<<std::endl;
                    nbr_vox++;
                }
                //else
                //    m_octree->add_voxel(x,y,z,palette[v][0],palette[v][1],palette[v][2],100);
            }

    std::cout<<"Total: "<<xyzsiz<<" voxels"<<std::endl;

    std::cout<<"Simple: "<<nbr_vox<<" rendered voxels"<<std::endl;

    return true;
}



bool VoxImg::load_from_ply (std::string filnam,unsigned short _xsiz, unsigned short _ysiz, unsigned short _zsiz)///create voximg from a ply point cloud
{
    std::cout<<"enter load_from_ply"<<std::endl;
    xsiz=_xsiz;
    ysiz=_ysiz;
    zsiz=_zsiz;
    xyzsiz=xsiz*ysiz*zsiz;
    yzsiz=ysiz*zsiz;


    FILE *fil;
    
    //unsigned char *voxels;

    fil = fopen(filnam.c_str(),"rb"); if (!fil) return(false);

    PlyFile * ply;
    ply=read_ply(fil);
    //...
    std::cout<<"load_from_ply OK"<<std::endl;


    return true;
}

