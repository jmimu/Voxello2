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
    xyzsiz=xsiz*ysiz*zsiz;

    std::cout<<filnam<<": got "<<xsiz_tmp<<"*"<<ysiz_tmp<<"*"<<zsiz_tmp<<" voxels_tmp"<<std::endl;

#ifdef VOX_24BIT
    unsigned char *voxels_tmp_8b = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));//before rotation, 8bit (like in the file)
    unsigned int *voxels_tmp = (unsigned int *)malloc(xyzsiz*sizeof(unsigned int));//before rotation
    voxels = (unsigned int *)malloc(xyzsiz*sizeof(unsigned int));
#else
    unsigned char *voxels_tmp = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));//before rotation
    voxels = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));
#endif

    if (!voxels_tmp)
        std::cout<<"Error voxels_tmp"<<std::endl;
    if (!voxels)
        std::cout<<"Error voxels"<<std::endl;

#ifdef VOX_24BIT
    fread(voxels_tmp_8b,xyzsiz,1,fil); //The 3-D array itself!
#else
    fread(voxels_tmp,xyzsiz,1,fil); //The 3-D array itself!
#endif

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
    
#ifdef VOX_24BIT
    for (long i=0;i<xyzsiz;i++)
    {
        if (voxels_tmp_8b[i]!=0xff)
            voxels_tmp[i]=(((unsigned int)palette[voxels_tmp_8b[i]][0])<<16)+(((unsigned int)palette[voxels_tmp_8b[i]][1])<<8)+(((unsigned int)palette[voxels_tmp_8b[i]][2])<<0);
        else voxels_tmp[i]=0xff;
        //voxels_tmp[i]=(((unsigned int)palette[voxels_tmp_8b[i]][0])<<16);
    }
#endif

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
#ifdef VOX_24BIT
    free(voxels_tmp_8b);
#endif
    free(voxels_tmp);

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



typedef struct Vertex {
  float x,y,z;
  float r,g,b;
  float nx,ny,nz;
  void *other_props;       /* other properties */
} Vertex;

static int nverts;
static Vertex **vlist;

PlyProperty vert_props[] = { /* list of property information for a vertex */
  {(char *)"x", Float32, Float32, offsetof(Vertex,x), 0, 0, 0, 0},
  {(char *)"y", Float32, Float32, offsetof(Vertex,y), 0, 0, 0, 0},
  {(char *)"z", Float32, Float32, offsetof(Vertex,z), 0, 0, 0, 0},
  {(char *)"red", Float32, Float32, offsetof(Vertex,r), 0, 0, 0, 0},
  {(char *)"green", Float32, Float32, offsetof(Vertex,g), 0, 0, 0, 0},
  {(char *)"blue", Float32, Float32, offsetof(Vertex,b), 0, 0, 0, 0}
};



///if only one size is given, it will be used for max dimension. the other will be proportional to model size.
bool VoxImg::load_from_ply (std::string filnam,unsigned short _xsiz, unsigned short _ysiz, unsigned short _zsiz)///create voximg from a ply point cloud
{
    std::vector<std::string> filnames;
    filnames.push_back(filnam);
    return load_from_ply (filnames,_xsiz, _ysiz, _zsiz);
}

bool VoxImg::load_from_ply (std::vector<std::string> filnames,unsigned short _xsiz, unsigned short _ysiz, unsigned short _zsiz)///create voximg from a ply point cloud
{
    std::cout<<"enter load_from_ply--------------------------------------------------"<<std::endl;
    
    std::vector<Vertex *>vlist_total;
    
    for (int i=0;i<255;i++)
    {
#ifdef PALETTE_RRGGGBBB
        //colors : RRGGGBBB
        palette[i][0]=i&192;
        palette[i][1]=((i>>3)&7)<<5;
        palette[i][2]=(i&7)<<5;
#endif
#ifdef PALETTE_RRRGGGBB
        //colors : RRRGGGBB
        palette[i][0]=i&224;
        palette[i][1]=((i>>2)&7)<<5;
        palette[i][2]=(i&3)<<6;
#endif
    }
        /*palette[1][0]=0;
        palette[1][1]=0;
        palette[1][2]=120;
        palette[6][0]=0;
        palette[6][1]=120;
        palette[6][2]=0;*/
  
  std::vector<std::string>::iterator filnam_it;
  for (filnam_it=filnames.begin() ; filnam_it < filnames.end(); filnam_it++)
  {
    FILE *fil;
    
    fil = fopen((*filnam_it).c_str(),"rb"); if (!fil) return(false);

    PlyFile * ply;
    ply=read_ply(fil);
    //std::cout<<"num_elem_types: "<<ply->num_elem_types<<std::endl;
    int elem_count,i,j;
    for (i = 0; i < ply->num_elem_types; i++) 
    {
      char * elem_name = setup_element_read_ply (ply, i, &elem_count);
      //std::cout<<"nb elements: "<<elem_count<<std::endl;
      if (equal_strings ((char *)"vertex", elem_name))
      {
          /*
            Create a vertex list to hold all the vertices.
          */
          vlist = (Vertex **) malloc (sizeof (Vertex *) * elem_count);
          nverts = elem_count;
          std::cout<<"Nb verts: "<<nverts<<". Reading file..."<<std::endl;
          setup_property_ply (ply, &vert_props[0]);
          setup_property_ply (ply, &vert_props[1]);
          setup_property_ply (ply, &vert_props[2]);
          for (j = 0; j < ply->elems[i]->nprops; j++) 
          {
            PlyProperty *prop;
            prop = ply->elems[i]->props[j];
            if (equal_strings ((char *)"red", prop->name))
              setup_property_ply (ply, &vert_props[3]);
            if (equal_strings ((char *)"green", prop->name))
              setup_property_ply (ply, &vert_props[4]);
            if (equal_strings ((char *)"blue", prop->name))
              setup_property_ply (ply, &vert_props[5]);
          }
          for (j = 0; j < elem_count; j++) {
            //std::cout<<".";
            vlist[j] = (Vertex *) malloc (sizeof (Vertex));
            vlist[j]->r = 1;
            vlist[j]->g = 1;
            vlist[j]->b = 1;
            get_element_ply (ply, (void *) vlist[j]);
            vlist_total.push_back(vlist[j]);
          }
          //std::cout<<std::endl;
      }
    }
    close_ply (ply);
    free_ply (ply);
  }
    std::cout<<"Calculate model size..."<<std::endl;


    //find ply size
    float x_min=vlist_total[0]->x;
    float x_max=vlist_total[0]->x;
    float y_min=vlist_total[0]->y;
    float y_max=vlist_total[0]->y;
    float z_min=vlist_total[0]->z;
    float z_max=vlist_total[0]->z;
    for (long j = 1; j < vlist_total.size(); j++) {
        if (vlist_total[j]->x > x_max) x_max=vlist_total[j]->x;
        if (vlist_total[j]->x < x_min) x_min=vlist_total[j]->x;
        if (vlist_total[j]->y > y_max) y_max=vlist_total[j]->y;
        if (vlist_total[j]->y < y_min) y_min=vlist_total[j]->y;
        if (vlist_total[j]->z > z_max) z_max=vlist_total[j]->z;
        if (vlist_total[j]->z < z_min) z_min=vlist_total[j]->z;
    }

    std::cout<<"Cloud size: ("<<x_min<<","<<y_min<<","<<z_min<<") - ("<<x_max<<","<<y_max<<","<<z_max<<")"<<std::endl;
/*    x_min=-35;
    x_max=17;
    y_min=-8;
    y_max= 7;
    z_min=-2;
    z_max= 8;*/

    if (_zsiz<1)
    {
        if (((x_max-x_min)>=(y_max-y_min))&&((x_max-x_min)>=(z_max-z_min)))
        {
            xsiz=_xsiz;
            ysiz=_xsiz*(y_max-y_min)/(x_max-x_min);
            zsiz=_xsiz*(z_max-z_min)/(x_max-x_min);
        }
        if (((y_max-y_min)>=(x_max-x_min))&&((y_max-y_min)>=(z_max-z_min)))
        {
            ysiz=_xsiz;
            xsiz=_xsiz*(x_max-x_min)/(y_max-y_min);
            zsiz=_xsiz*(z_max-z_min)/(y_max-y_min);
        }
        if (((z_max-z_min)>=(x_max-x_min))&&((z_max-z_min)>=(y_max-y_min)))
        {
            zsiz=_xsiz;
            xsiz=_xsiz*(x_max-x_min)/(z_max-z_min);
            ysiz=_xsiz*(y_max-y_min)/(z_max-z_min);
        }
    }else{
        xsiz=_xsiz;
        ysiz=_ysiz;
        zsiz=_zsiz;
    }
    xyzsiz=xsiz*ysiz*zsiz;
    yzsiz=ysiz*zsiz;

#ifdef VOX_24BIT
    voxels = (unsigned int *)malloc(xyzsiz*sizeof(unsigned int));
#else
    voxels = (unsigned char *)malloc(xyzsiz*sizeof(unsigned char));
#endif
    //empty voxels
    for (long i=0;i<xyzsiz;i++)
      voxels[i]=255;


    std::cout<<"Voxelization..."<<std::endl;
#ifdef VOX_24BIT
    unsigned int v;
#else
    unsigned char v;
#endif
    long x,y,z;
    float x_maxmin=x_max-x_min+0.00001;
    float y_maxmin=y_max-y_min+0.00001;
    float z_maxmin=z_max-z_min+0.00001;
    //for each point modify a voxel
    for (long j = 0; j < vlist_total.size(); j++) {
        //std::cout<<vlist[j]->x<<" "<<vlist[j]->y<<" "<<vlist[j]->z<<" "<<vlist[j]->r<<" "<<vlist[j]->g<<" "<<vlist[j]->b<<"\n";
        x=xsiz*(vlist_total[j]->x-x_min)/x_maxmin;
        y=ysiz*(vlist_total[j]->y-y_min)/y_maxmin;
        z=zsiz-zsiz*(vlist_total[j]->z-z_min)/z_maxmin-1;
        if ((x<0)||(x>=xsiz)||(y<0)||(y>=ysiz)||(z<0)||(z>=zsiz)) continue;
        //std::cout<<"vox "<<x<<" "<<y<<" "<<z<<"... "<<std::flush;



#ifdef VOX_24BIT
        //v=((unsigned int)vlist[j]->r)<<16+((unsigned int)vlist[j]->g)<<8+((unsigned int)vlist[j]->b);
        v=(((unsigned int)vlist_total[j]->r)<<16)+(((unsigned int)vlist_total[j]->g)<<8)+(((unsigned int)vlist_total[j]->b));
        //std::cout<<"to draw "<<((unsigned int)vlist[j]->r)<<" "<<((unsigned int)vlist[j]->g)<<" "<<((unsigned int)vlist[j]->b)<<" => "<<v<<"\n";
            //std::cout<<v<<" ";
#else
    #ifdef PALETTE_RRGGGBBB
            //colors RRGGGBBB
            v=(((unsigned int)vlist_total[j]->r)&192)+((((unsigned int)vlist_total[j]->g)>>2)&56)+(((unsigned int)vlist_total[j]->b)>>5);
    #endif
    #ifdef PALETTE_RRRGGGBB
            //colors RRGGGGBB
            v=(((unsigned int)vlist_total[j]->r)&224)+((((unsigned int)vlist_total[j]->g)>>3)&28)+(((unsigned int)vlist_total[j]->b)>>6);
    #endif
#endif


        voxels[x*yzsiz+y*zsiz+z]=v;
        //std::cout<<"done!"<<std::endl;
    }
    
    std::cout<<"load_from_ply OK--------------------------------------------------"<<std::endl;


    return true;
}

