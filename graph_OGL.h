#ifndef GRAPH_OGL_H
#define GRAPH_OGL_H

#include "graph.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define FPS 30

class Graph_OGL : public Graph
{
    public:
        Graph_OGL();
        virtual void init();
        virtual void line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b);
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b);
        virtual void quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned int v);
        virtual void close();
        virtual void start_frame();
        virtual void end_frame();
        virtual void create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c);//draw on a other buffer, not screen
        virtual void draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a);//paste image on the screen, coords are relative to screen center
        virtual void zoom_change(float delta);
        SDL_Surface * create_gl_surf(int width,int height);//create a SDL surface with OGP texture format
        GLuint glsurf2Texture(SDL_Surface * gl_surface,GLuint texture_ID);
        void clear_alpha_surf(SDL_Surface * surf);
        void dessineRectangle(double x,double y, double largeur,double hauteur,unsigned short r,unsigned short g,unsigned short b,unsigned short a);
    private:
        int picture_size;//must be 2^x
        SDL_Surface *screen;
        //about voxel rendering
        GLuint texture1;
        SDL_Surface * one_voxel_picture_big;//a surface where I draw a voxel, to be repetited (with oversampling)
        SDL_Surface * one_voxel_picture;//reduiced surface
        double oversampling;

};

#endif // GRAPH_OGL_H
