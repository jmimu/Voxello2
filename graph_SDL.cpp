#include "graph_SDL.h"


#include <iostream>


Graph_SDL::Graph_SDL():Graph(),screen(NULL),one_voxel_picture(NULL),surface_coverage()
{
    ORTHO_ZOOM=6.0;
}

void Graph_SDL::init()
{
    SDL_WM_SetCaption("SDL GL Application", NULL);
    screen=SDL_SetVideoMode(256*4, 192*4, 8,  SDL_HWPALETTE);
    if(!screen){
      printf("Couldn't set video mode: %s\n", SDL_GetError());
      exit(-1);
    }
    std::cout<<"Video mode BPP: "<<(int)screen->format->BytesPerPixel<<std::endl;
    
    //set palette
    SDL_Color colors[256];
    int i;
    /* Fill colors with color information */
    for(i=0;i<256;i++){
      colors[i].r=i;
      colors[i].g=i;
      colors[i].b=i;
    }
    SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
    

    one_voxel_picture=SDL_CreateRGBSurface(SDL_HWPALETTE /*SDL_HWSURFACE*/, SUB_PICTURE_SIZE, SUB_PICTURE_SIZE, 8, 0, 0, 0, 0);
    SDL_SetPalette(one_voxel_picture, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
}

void Graph_SDL::start_frame()
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}

void Graph_SDL::end_frame()
{
    SDL_Flip(screen);
    //SDL_Delay(1000/FPS);
}

void Graph_SDL::line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b)
{
    //lineColor(screen,x1,y1,x2,y2,color); 
    Draw_Line(screen,x1,y1,x2,y2,r);
}

void Graph_SDL::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b)
{
    filledTrigonColor(one_voxel_picture,x1,y1,x2,y2,x3,y3,r);
    filledTrigonColor(one_voxel_picture,x3,y3,x4,y4,x1,y1,r);
    /*Draw_Line(one_voxel_picture,x1,y1,x2,y2,color);
    Draw_Line(one_voxel_picture,x2,y2,x3,y3,color);
    Draw_Line(one_voxel_picture,x3,y3,x4,y4,color);
    Draw_Line(one_voxel_picture,x4,y4,x1,y1,color);*/
}

void Graph_SDL::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned int v)
{
    filledTrigonColor(one_voxel_picture,x1,y1,x2,y2,x3,y3,v);
    filledTrigonColor(one_voxel_picture,x3,y3,x4,y4,x1,y1,v);
    /*Draw_Line(one_voxel_picture,x1,y1,x2,y2,color);
    Draw_Line(one_voxel_picture,x2,y2,x3,y3,color);
    Draw_Line(one_voxel_picture,x3,y3,x4,y4,color);
    Draw_Line(one_voxel_picture,x4,y4,x1,y1,color);*/
}

void Graph_SDL::close()
{

}

void Graph_SDL::create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c)
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

    int pt0x=M_000_c.x*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    int pt0y=M_000_c.y*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    int pt1x=pt0x+vect_vox_x_c.x;
    int pt1y=pt0y+vect_vox_x_c.y;
    int pt2x=pt1x+vect_vox_y_c.x;
    int pt2y=pt1y+vect_vox_y_c.y;
    int pt3x=pt2x-vect_vox_x_c.x;
    int pt3y=pt2y-vect_vox_x_c.y;
    int pt4x=pt0x+vect_vox_z_c.x;
    int pt4y=pt0y+vect_vox_z_c.y;
    int pt5x=pt1x+vect_vox_z_c.x;
    int pt5y=pt1y+vect_vox_z_c.y;
    int pt6x=pt2x+vect_vox_z_c.x;
    int pt6y=pt2y+vect_vox_z_c.y;
    int pt7x=pt3x+vect_vox_z_c.x;
    int pt7y=pt3y+vect_vox_z_c.y;

    //compute min and max to minimize surface to blit
    int min_x=pt0x;if (min_x>pt1x) min_x=pt1x;if (min_x>pt2x) min_x=pt2x;if (min_x>pt3x) min_x=pt3x;
    if (min_x>pt4x) min_x=pt4x;if (min_x>pt5x) min_x=pt5x;if (min_x>pt6x) min_x=pt6x;if (min_x>pt7x) min_x=pt7x;
    int max_x=pt0x;if (max_x<pt1x) max_x=pt1x;if (max_x<pt2x) max_x=pt2x;if (max_x<pt3x) max_x=pt3x;
    if (max_x<pt4x) max_x=pt4x;if (max_x<pt5x) max_x=pt5x;if (max_x<pt6x) max_x=pt6x;if (max_x<pt7x) max_x=pt7x;
    int min_y=pt0y;if (min_y>pt1y) min_y=pt1y;if (min_y>pt2y) min_y=pt2y;if (min_y>pt3y) min_y=pt3y;
    if (min_y>pt4y) min_y=pt4y;if (min_y>pt5y) min_y=pt5y;if (min_y>pt6y) min_y=pt6y;if (min_y>pt7y) min_y=pt7y;
    int max_y=pt0y;if (max_y<pt1y) max_y=pt1y;if (max_y<pt2y) max_y=pt2y;if (max_y<pt3y) max_y=pt3y;
    if (max_y<pt4y) max_y=pt4y;if (max_y<pt5y) max_y=pt5y;if (max_y<pt6y) max_y=pt6y;if (max_y<pt7y) max_y=pt7y;

    //SDL_FillRect(one_voxel_picture, NULL, SDL_MapRGB(one_voxel_picture->format, 0, 0, 0));
    SDL_FillRect(one_voxel_picture, NULL, 0xFF);
    //determine if face was to be drawn checking if the normal is looking in the camera direction (z>0)
    //face 1 normal = -vect z
    //face 2 normal = +vect z
    //face 3 normal = +vect x
    //face 4 normal = +vect y
    //face 5 normal = -vect x
    //face 6 normal = -vect y
    if (-vect_vox_z_c.z>0)
        quad(pt0x,pt0y,pt1x,pt1y,pt2x,pt2y,pt3x,pt3y,0x50);
    if (+vect_vox_z_c.z>0)
        quad(pt4x,pt4y,pt5x,pt5y,pt6x,pt6y,pt7x,pt7y,0x70);
    if (+vect_vox_x_c.z>0)
        quad(pt1x,pt1y,pt2x,pt2y,pt6x,pt6y,pt5x,pt5y,0x90);
    if (+vect_vox_y_c.z>0)
        quad(pt2x,pt2y,pt3x,pt3y,pt7x,pt7y,pt6x,pt6y,0xB0);
    if (-vect_vox_x_c.z>0)
        quad(pt0x,pt0y,pt3x,pt3y,pt7x,pt7y,pt4x,pt4y,0xD0);
    if (-vect_vox_y_c.z>0)
        quad(pt1x,pt1y,pt0x,pt0y,pt4x,pt4y,pt5x,pt5y,0xF0);
    SDL_SetColorKey(one_voxel_picture, SDL_SRCCOLORKEY, 0xFF);

    //blit only surface_coverage
    surface_coverage.x=min_x;
    surface_coverage.y=min_y;
    surface_coverage.w=max_x-min_x;
    surface_coverage.h=max_y-min_y;
}

void Graph_SDL::draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a)
{
    SDL_Rect position;
    position.x=(LARGEUR_FENETRE>>1)-(SUB_PICTURE_SIZE>>1)+surface_coverage.x+x;
    position.y=(HAUTEUR_FENETRE>>1)-(SUB_PICTURE_SIZE>>1)+surface_coverage.y+y;
    //std::cout<<"draw: "<<position.x<<" "<<position.y<<"\n";
    SDL_BlitSurface(one_voxel_picture, &surface_coverage, screen , &position);//very slow!
}
    
