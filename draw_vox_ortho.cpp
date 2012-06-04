#include <iostream>
#include "draw_vox_ortho.h"

#define SPACE_SIZE 8

double pseudo_normalize(double x,double ORTHO_ZOOM,double scale)
{
    return x/ORTHO_ZOOM;
/*    if (x==0.0) return 0;
    return (x/fabs(x)*scale);*/
}


Draw_Vox_Ortho::Draw_Vox_Ortho(Graph* graph,VoxImg* obj):
    vect_vox_x_c(),vect_vox_y_c(),vect_vox_z_c(),
    m_graph(graph),m_obj(obj),
    M_000_v(0,0,0),M_100_v(1,0,0),M_010_v(0,1,0),M_001_v(0,0,1),
    M_000_c(),M_100_c(),M_010_c(),M_001_c(),
    rotation_center_v(m_obj->get_xsiz()>>1,m_obj->get_ysiz()>>1,m_obj->get_zsiz()>>1),
    CamRot_c2v(),CamPos_c(0,0,-10)
{
}

void Draw_Vox_Ortho::render()
{
    m_graph->start_frame();
    draw_voxels();
    
/*    m_graph->line(M_000_c.x*m_graph->ORTHO_ZOOM+512-5,M_000_c.y*m_graph->ORTHO_ZOOM+384-5,M_000_c.x*m_graph->ORTHO_ZOOM+512+5,M_000_c.y*m_graph->ORTHO_ZOOM+384+5,0xFF,0xFF,0xFF);
    m_graph->line(M_000_c.x*m_graph->ORTHO_ZOOM+512-5,M_000_c.y*m_graph->ORTHO_ZOOM+384+5,M_000_c.x*m_graph->ORTHO_ZOOM+512+5,M_000_c.y*m_graph->ORTHO_ZOOM+384-5,0xFF,0xFF,0xFF);
    m_graph->line(M_100_c.x*m_graph->ORTHO_ZOOM+512-5,M_100_c.y*m_graph->ORTHO_ZOOM+384-5,M_100_c.x*m_graph->ORTHO_ZOOM+512+5,M_100_c.y*m_graph->ORTHO_ZOOM+384+5,0xFF,0xFF,0xFF);
    m_graph->line(M_100_c.x*m_graph->ORTHO_ZOOM+512-5,M_100_c.y*m_graph->ORTHO_ZOOM+384+5,M_100_c.x*m_graph->ORTHO_ZOOM+512+5,M_100_c.y*m_graph->ORTHO_ZOOM+384-5,0xFF,0xFF,0xFF);
    m_graph->line(M_010_c.x*m_graph->ORTHO_ZOOM+512-5,M_010_c.y*m_graph->ORTHO_ZOOM+384-5,M_010_c.x*m_graph->ORTHO_ZOOM+512+5,M_010_c.y*m_graph->ORTHO_ZOOM+384+5,0xFF,0xFF,0xFF);
    m_graph->line(M_010_c.x*m_graph->ORTHO_ZOOM+512-5,M_010_c.y*m_graph->ORTHO_ZOOM+384+5,M_010_c.x*m_graph->ORTHO_ZOOM+512+5,M_010_c.y*m_graph->ORTHO_ZOOM+384-5,0xFF,0xFF,0xFF);
    m_graph->line(M_001_c.x*m_graph->ORTHO_ZOOM+512-5,M_001_c.y*m_graph->ORTHO_ZOOM+384-5,M_001_c.x*m_graph->ORTHO_ZOOM+512+5,M_001_c.y*m_graph->ORTHO_ZOOM+384+5,0xFF,0xFF,0xFF);
    m_graph->line(M_001_c.x*m_graph->ORTHO_ZOOM+512-5,M_001_c.y*m_graph->ORTHO_ZOOM+384+5,M_001_c.x*m_graph->ORTHO_ZOOM+512+5,M_001_c.y*m_graph->ORTHO_ZOOM+384-5,0xFF,0xFF,0xFF);*/
    
/*    
    //draw a lot of points...
    Pt3d M_v;
    Pt2d M_c;
    for (int x=0;x<=SPACE_SIZE;x++)
        for (int y=0;y<=SPACE_SIZE;y++)
            for (int z=0;z<=SPACE_SIZE;z++)
            {
                M_v.x=x;
                M_v.y=y;
                M_v.z=z;
                M_v.mult_proj(CamRot_c2v,&M_c);//is is faster than Gauss?
                draw_pt(M_c);
            }
  */  

    m_graph->end_frame();
}

void Draw_Vox_Ortho::update_camera(double x,double y,double z,double ang_x,double ang_y,double ang_z)
{
    CamPos_c.x=x;
    CamPos_c.y=y;
    CamPos_c.z=z;

    CamRot_c2v=Mat3x3::matrice_rot(ang_x,ang_y,ang_z);
    M_000_v.mult(CamRot_c2v,&M_000_c);
    M_100_v.mult(CamRot_c2v,&M_100_c);
    M_010_v.mult(CamRot_c2v,&M_010_c);
    M_001_v.mult(CamRot_c2v,&M_001_c);

    //compute base vectors of voxels frame on screen
    M_100_c.sub(M_000_c,&vect_vox_x_c);
    M_010_c.sub(M_000_c,&vect_vox_y_c);
    M_001_c.sub(M_000_c,&vect_vox_z_c);
    vect_vox_x_c.scal_mult(m_graph->ORTHO_ZOOM);
    vect_vox_y_c.scal_mult(m_graph->ORTHO_ZOOM);
    vect_vox_z_c.scal_mult(m_graph->ORTHO_ZOOM);
/*    vect_vox_x_c.x=int(vect_vox_x_c.x);
    vect_vox_x_c.y=int(vect_vox_x_c.y);
    vect_vox_x_c.z=int(vect_vox_x_c.z);
    vect_vox_y_c.x=int(vect_vox_y_c.x);
    vect_vox_y_c.y=int(vect_vox_y_c.y);
    vect_vox_y_c.z=int(vect_vox_y_c.z);
    vect_vox_z_c.x=int(vect_vox_z_c.x);
    vect_vox_z_c.y=int(vect_vox_z_c.y);
    vect_vox_z_c.z=int(vect_vox_z_c.z);*/



}

void Draw_Vox_Ortho::draw_pt(const Pt2d &p)
{
    m_graph->line(p.x*m_graph->ORTHO_ZOOM+512-1,p.y*m_graph->ORTHO_ZOOM+384-1,p.x*m_graph->ORTHO_ZOOM+512+1,p.y*m_graph->ORTHO_ZOOM+384+1,0xCC,0x50,0x50);
    m_graph->line(p.x*m_graph->ORTHO_ZOOM+512-1,p.y*m_graph->ORTHO_ZOOM+384+1,p.x*m_graph->ORTHO_ZOOM+512+1,p.y*m_graph->ORTHO_ZOOM+384-1,0xCC,0x50,0x50);
}


//it could be interesting to make 8 cases for every first voxel drawn, to avoid axis_dir[0]* and simplfy cumulative indexes computation
void Draw_Vox_Ortho::draw_voxels()
{
    /*
      axes :
              y
              |/
            -- -- x
             /|
           z  

      points :
           3   2
        7   6
           0   1
        4   5
        
      faces :
              4  1
              |/
          5 -- -- 3
             /|
           2  6
    */
    m_graph->create_one_voxel_picture(M_000_c,vect_vox_x_c,vect_vox_y_c,vect_vox_z_c);

    int position_x=0;
    int position_y=0;
    int position_z=0;//deep
    float dark=0;
    int alpha=255;


    //to draw all voxels in right order
    int axis_dir[3];//-1 or +1
    int axis_begin[3];//0 or m_obj->xsiz, only depends on axis_dir
    int axis_end[3];//0 or m_obj->xsiz, only depends on axis_dir
    //test on vectors to find if x (0), y (1) or z (2) is to be first or not
    //What must be determined is which voxel must be drawn first : 0,0,0, or m_obj->xsiz-1,0,0 ??
    //=>what is important is axis_dir ! => depends on which face of frame voxel is drawn <=> normals orientations
    axis_dir[0]=1;
    axis_dir[1]=1;
    axis_dir[2]=1;
    if (vect_vox_x_c.z<0) axis_dir[0]=-1;
    if (vect_vox_y_c.z<0) axis_dir[1]=-1;
    if (vect_vox_z_c.z<0) axis_dir[2]=-1;
 
    axis_begin[0]=0;if (axis_dir[0]==-1) axis_begin[0]=m_obj->get_xsiz()-1;
    axis_begin[1]=0;if (axis_dir[1]==-1) axis_begin[1]=m_obj->get_ysiz()-1;
    axis_begin[2]=0;if (axis_dir[2]==-1) axis_begin[2]=m_obj->get_zsiz()-1;
    axis_end[0]=0;if (axis_dir[0]==1) axis_end[0]=m_obj->get_xsiz()-1;
    axis_end[1]=0;if (axis_dir[1]==1) axis_end[1]=m_obj->get_ysiz()-1;
    axis_end[2]=0;if (axis_dir[2]==1) axis_end[2]=m_obj->get_zsiz()-1;


    int axis_end_times_axis_dir[3];//TODO: mabe improve with an if instead of *
    axis_end_times_axis_dir[0]=axis_end[0]*axis_dir[0];
    axis_end_times_axis_dir[1]=axis_end[1]*axis_dir[1];
    axis_end_times_axis_dir[2]=axis_end[2]*axis_dir[2];

#ifdef VOX_24BIT
    unsigned int v;//value of current voxel
#else
    unsigned char v;//value of current voxel
#endif
    bool hidden;
    long obj_i=0;
    long obj_i_diff_X=axis_dir[0]*m_obj->get_yzsiz();//what to add to get neighbors in the 3 directions (for NEIGH_OPTIMIZATION)
    long obj_i_diff_Y=axis_dir[1]*m_obj->get_zsiz();//what to add to get neighbors in the 3 directions (for NEIGH_OPTIMIZATION)
    long obj_i_diff_Z=axis_dir[2];//what to add to get neighbors in the 3 directions (for NEIGH_OPTIMIZATION)

    //initial position on camera frame
    /*double position_x_xref=-rotation_center_v.x*vect_vox_x_c.x;
    double position_x_yref=-rotation_center_v.y*vect_vox_y_c.x;
    double position_x_zref=-rotation_center_v.z*vect_vox_z_c.x;*/
    float ORTHO_ZOOM_times_LIGHT_DIST=m_graph->ORTHO_ZOOM*LIGHT_DIST;
    float inv_ORTHO_ZOOM_times_LIGHT_DIST=1.0/(m_graph->ORTHO_ZOOM*LIGHT_DIST);
    
    for (int x=axis_begin[0];x*axis_dir[0]<=axis_end[0]*axis_dir[0];x+=axis_dir[0])
    {
        for (int y=axis_begin[1];y*axis_dir[1]<=axis_end[1]*axis_dir[1];y+=axis_dir[1])
        {
            for (int z=axis_begin[2];z*axis_dir[2]<=axis_end[2]*axis_dir[2];z+=axis_dir[2])
            {
                obj_i=x*m_obj->get_ysiz()*m_obj->get_zsiz()+y*m_obj->get_zsiz()+z;//TODO optimize this (by making cases for each direction ?)
                v=m_obj->voxels[obj_i];
                if (v==0xFF) {obj_i++;continue;}
#ifdef NEIGH_OPTIMIZATION
                //optimization: if the 3 neigh. voxels in camera direction are not empty, no need to draw this one !
                hidden=true;
                if (x*axis_dir[0]<axis_end_times_axis_dir[0])//TODO: mabe improve with an if instead of *
                {
                    //if (m_obj->voxels[(x+axis_dir[0])*m_obj->get_ysiz()*m_obj->get_zsiz()+y*m_obj->get_zsiz()+z]==0xFF) hidden=false;
                    if (m_obj->voxels[obj_i+obj_i_diff_X]==0xFF) hidden=false;
                }else hidden=false;
                if (y*axis_dir[1]<axis_end_times_axis_dir[1])//TODO: mabe improve with an if instead of *
                {
                    //if (m_obj->voxels[x*m_obj->get_ysiz()*m_obj->get_zsiz()+(y+axis_dir[1])*m_obj->get_zsiz()+z]==0xFF) hidden=false;
                    if (m_obj->voxels[obj_i+obj_i_diff_Y]==0xFF) hidden=false;
                }else hidden=false;
                if (z*axis_dir[2]<axis_end_times_axis_dir[2])//TODO: mabe improve with an if instead of *
                {
                    //if (m_obj->voxels[x*m_obj->get_ysiz()*m_obj->get_zsiz()+y*m_obj->get_zsiz()+(z+axis_dir[2])]==0xFF) hidden=false;
                    if (m_obj->voxels[obj_i+obj_i_diff_Z]==0xFF) hidden=false;
                }else hidden=false;
                if (hidden) continue;
#endif
                //TODO: improve position computation !
                position_x=(x-rotation_center_v.x)*vect_vox_x_c.x+(y-rotation_center_v.y)*vect_vox_y_c.x+(z-rotation_center_v.z)*vect_vox_z_c.x;
                position_y=(x-rotation_center_v.x)*vect_vox_x_c.y+(y-rotation_center_v.y)*vect_vox_y_c.y+(z-rotation_center_v.z)*vect_vox_z_c.y;
                position_z=(x-rotation_center_v.x)*vect_vox_x_c.z+(y-rotation_center_v.y)*vect_vox_y_c.z+(z-rotation_center_v.z)*vect_vox_z_c.z;
                alpha=255.0;
                if (position_z>0)
                {
                    dark=1.0;
                    if (position_z>ALPHA_DIST)
                        alpha=2550.0/((position_z-(ALPHA_DIST))+10);
                }else{
                    dark=(-position_z+ORTHO_ZOOM_times_LIGHT_DIST)*inv_ORTHO_ZOOM_times_LIGHT_DIST;
                    alpha=255;
                }
#ifdef VOX_24BIT

                m_graph->draw_1_voxel(position_x,position_y,(v>>16)/dark,((v>>8)&0xFF)/dark,((v)&0xFF)/dark,alpha);
#else
                m_graph->draw_1_voxel(position_x,position_y,m_obj->palette[v][0]/dark,m_obj->palette[v][1]/dark,m_obj->palette[v][1]/dark,alpha);
#endif
                obj_i++;
            }
        }
    }

    //m_graph->draw_1_voxel(0,0,0xFF,0xFF,0xFF);
}

