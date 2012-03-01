#ifndef GRAPH_SDL_H
#define GRAPH_SDL_H

#include "graph.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "SDL_draw/SDL_draw.h"

#define FPS 30

class Graph_SDL : public Graph
{
    public:
        Graph_SDL();
        virtual void init();
        virtual void line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b);
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b);
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short v);
        virtual void close();
        virtual void start_frame();
        virtual void end_frame();
        virtual void create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c);//draw on a other buffer, not screen
        virtual void draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a);//paste image on the screen, coords are relative to screen center

    private:
        SDL_Surface *screen;
        //about voxel rendering
        SDL_Surface * one_voxel_picture;//a big surface where I draw a voxel, to be repetited
        SDL_Rect surface_coverage;//area to copy

};

#endif // GRAPH_SDL_H
