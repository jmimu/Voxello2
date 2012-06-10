#ifndef GRAPH_H
#define GRAPH_H

#include "pt3d.h"
/***
    Generic class to handle :
     - window opening
     - primitive drawing
     - FPS calculating & managing (later...)

*/

//#define ORTHO_ZOOM 6.0
//for the one-voxel picture, max side size in pixel
//it has to be >1.8*ORTHO_ZOOM  (sqrt(3)*ORTHO_ZOOM)
#define SUB_PICTURE_SIZE 64


inline double add_one_in_dir(double x)
{
    return x;
    /*if (x>1) return x*(ORTHO_ZOOM+1.0)/ORTHO_ZOOM;
    if (x<1) return x*(ORTHO_ZOOM+1.0)/ORTHO_ZOOM;*/
    return 0;
}

class Graph
{
    public:
        Graph(){};
        virtual void init()=0;
        virtual void line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b)=0;
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b)=0;
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned int v)=0;
        virtual void close()=0;
        virtual void start_frame()=0;
        virtual void end_frame()=0;
        virtual void zoom_change(float delta)=0;
        
        virtual void create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c)=0;//draw on a other buffer, not screen
        virtual void draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a)=0;//paste image on the screen, coords are relative to screen center
        float ORTHO_ZOOM;
    private:
        //the voxel picture
        //the voxel picture size and offset


};

#endif // GRAPH_H
