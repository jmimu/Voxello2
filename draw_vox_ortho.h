#ifndef DRAW_VOX_ORTHO_H
#define DRAW_VOX_ORTHO_H

#include "graph.h"
#include "voximg.h"
#include "pt3d.h"
#include "mat3x3.h"

/**
    To draw voxels objects
    This class is only for othometric projection
  */

/***
 2 frames
  - Vox frame : natural frame of voxels (variables finish with _v)
  - Camera frame : rotated, where XY is the screen frame (variables finish with _c)
*/

#define NEIGH_OPTIMIZATION
#define LIGHT_DIST 100.0
#define ALPHA_DIST 800.0

//normalize
double pseudo_normalize(double x,double ORTHO_ZOOM,double scale=1.0);

class Draw_Vox_Ortho
{
    public:
        Draw_Vox_Ortho(Graph* graph,VoxImg* obj);
        void render();//give a voxel object !
        void update_camera(double x,double y,double z,double ang_x,double ang_y,double ang_z);
        void draw_pt(const Pt2d &p);
        Pt3d vect_vox_x_c,vect_vox_y_c,vect_vox_z_c;//the vector you have to move of on screen when you move in voxel space
    private:
        void draw_voxels();
        Graph * m_graph;
        VoxImg * m_obj;
        Pt3d M_000_v,M_100_v,M_010_v,M_001_v;//4 points to have the voxel frame in camera frame (in cam frame)
        Pt3d M_000_c,M_100_c,M_010_c,M_001_c;//(in vox frame)
        Pt3d rotation_center_v;//where the camera aims
        Mat3x3 CamRot_c2v;
        Pt3d CamPos_c;

};

#endif // DRAW_VOX_ORTHO_H
