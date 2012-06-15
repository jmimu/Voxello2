#include "graph_OGL.h"


#include <iostream>



Graph_OGL::Graph_OGL():Graph(),screen(NULL),one_voxel_picture_big(0),one_voxel_picture(0)
{
    ORTHO_ZOOM=6.0;
    picture_size=64;
    oversampling=2.0;
}

void Graph_OGL::init()
{
    SDL_WM_SetCaption("SDL GL Application", 0);
    screen=SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32,  SDL_OPENGL);
    if(!screen){
      printf("Couldn't set video mode: %s\n", SDL_GetError());
      exit(-1);
    }
    std::cout<<"Video mode BPP: "<<(int)screen->format->BytesPerPixel<<std::endl;
    
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    //gluOrtho2D(0,LARGEUR_FENETRE,0,HAUTEUR_FENETRE);
    gluOrtho2D(0,LARGEUR_FENETRE,HAUTEUR_FENETRE,0);
    glEnable(GL_TEXTURE_2D);
    
    //for alpha
    glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ; 
    //clear color
    //glClearColor(0.2,0.2,0.5,1);
    glClearColor(0,0,0,1);

    //zoom_change(0);//to create one_voxel_picture
    one_voxel_picture_big=create_gl_surf(picture_size*oversampling, picture_size*oversampling);
    glGenTextures(1, &texture1);
}

SDL_Surface * Graph_OGL::create_gl_surf(int width,int height)
{
    SDL_Surface *gl_surface = NULL;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    gl_surface = SDL_CreateRGBSurface (SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
    return gl_surface;
}


GLuint Graph_OGL::glsurf2Texture(SDL_Surface * gl_surface,GLuint texture_ID)
{
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_surface->w,
                 gl_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                 gl_surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    return texture_ID;
}

void Graph_OGL::zoom_change(float delta)
{
    ORTHO_ZOOM+=delta;
    picture_size=16;
    if (ORTHO_ZOOM>4) picture_size=32;
    if (ORTHO_ZOOM>9) picture_size=64;
    if (ORTHO_ZOOM>19) picture_size=128;
    if (ORTHO_ZOOM>38) picture_size=256;
    if (ORTHO_ZOOM>76) picture_size=512;
    if (one_voxel_picture_big!=0)
      SDL_FreeSurface(one_voxel_picture_big);
    one_voxel_picture_big=create_gl_surf(picture_size*oversampling, picture_size*oversampling);
}

void Graph_OGL::start_frame()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glBegin(GL_QUADS);
}

void Graph_OGL::end_frame()
{
    glEnd();
    SDL_GL_SwapBuffers();
}

void Graph_OGL::clear_alpha_surf(SDL_Surface * surf)
{
    long n=surf->w*surf->h;
    unsigned int * p =((unsigned int*)surf->pixels);
    for (long i=0;i<n;i++)
    {
      (*p)=0;
      p++;
    }
}

void Graph_OGL::line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b)
{
    lineColor(screen,x1,y1,x2,y2,(r<<24)+(g<<16)+(b<<8)+0xFF); 
}

void Graph_OGL::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b)
{
    filledTrigonColor(one_voxel_picture_big,x1,y1,x2,y2,x3,y3,r);
    filledTrigonColor(one_voxel_picture_big,x3,y3,x4,y4,x1,y1,r);
    /*Draw_Line(one_voxel_picture,x1,y1,x2,y2,color);
    Draw_Line(one_voxel_picture,x2,y2,x3,y3,color);
    Draw_Line(one_voxel_picture,x3,y3,x4,y4,color);
    Draw_Line(one_voxel_picture,x4,y4,x1,y1,color);*/
}

void Graph_OGL::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned int c)
{
    unsigned int c2=c*0x01010100+0xFF;
    filledTrigonColor(one_voxel_picture_big,x1,y1,x2,y2,x3,y3,c2);
    filledTrigonColor(one_voxel_picture_big,x3,y3,x4,y4,x1,y1,c2);
    
    lineColor(one_voxel_picture_big,x1,y1,x2,y2,0x808080FF);
    lineColor(one_voxel_picture_big,x2,y2,x3,y3,0x808080FF);
    lineColor(one_voxel_picture_big,x3,y3,x4,y4,0x808080FF);
    lineColor(one_voxel_picture_big,x4,y4,x1,y1,0x808080FF);

    lineColor(one_voxel_picture_big,x1-1,y1,x2-1,y2,0x808080FF);
    lineColor(one_voxel_picture_big,x1,y1-1,x2,y2-1,0x808080FF);
    
    lineColor(one_voxel_picture_big,x2-1,y2,x3-1,y3,0x808080FF);
    lineColor(one_voxel_picture_big,x2,y2-1,x3,y3-1,0x808080FF);
    
    lineColor(one_voxel_picture_big,x3-1,y3,x4-1,y4,0x808080FF);
    lineColor(one_voxel_picture_big,x3,y3-1,x4,y4-1,0x808080FF);
    
    lineColor(one_voxel_picture_big,x4-1,y4,x1-1,y1,0x808080FF);
    lineColor(one_voxel_picture_big,x4,y4-1,x1,y1-1,0x808080FF);
}

void Graph_OGL::close()
{
    SDL_FreeSurface(one_voxel_picture_big);
    SDL_FreeSurface(one_voxel_picture);
}

void Graph_OGL::create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c)
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
    
    clear_alpha_surf(one_voxel_picture_big);
    
    int pt0x=M_000_c.x*ORTHO_ZOOM+(picture_size>>1)*oversampling ;
    int pt0y=M_000_c.y*ORTHO_ZOOM+(picture_size>>1)*oversampling ;
    int pt1x=pt0x+vect_vox_x_c.x*oversampling;
    int pt1y=pt0y+vect_vox_x_c.y*oversampling;
    int pt2x=pt1x+vect_vox_y_c.x*oversampling;
    int pt2y=pt1y+vect_vox_y_c.y*oversampling;
    int pt3x=pt2x-vect_vox_x_c.x*oversampling;
    int pt3y=pt2y-vect_vox_x_c.y*oversampling;
    int pt4x=pt0x+vect_vox_z_c.x*oversampling;
    int pt4y=pt0y+vect_vox_z_c.y*oversampling;
    int pt5x=pt1x+vect_vox_z_c.x*oversampling;
    int pt5y=pt1y+vect_vox_z_c.y*oversampling;
    int pt6x=pt2x+vect_vox_z_c.x*oversampling;
    int pt6y=pt2y+vect_vox_z_c.y*oversampling;
    int pt7x=pt3x+vect_vox_z_c.x*oversampling;
    int pt7y=pt3y+vect_vox_z_c.y*oversampling;

    //compute min and max to minimize surface to blit
    int min_x=pt0x;if (min_x>pt1x) min_x=pt1x;if (min_x>pt2x) min_x=pt2x;if (min_x>pt3x) min_x=pt3x;
    if (min_x>pt4x) min_x=pt4x;if (min_x>pt5x) min_x=pt5x;if (min_x>pt6x) min_x=pt6x;if (min_x>pt7x) min_x=pt7x;
    int max_x=pt0x;if (max_x<pt1x) max_x=pt1x;if (max_x<pt2x) max_x=pt2x;if (max_x<pt3x) max_x=pt3x;
    if (max_x<pt4x) max_x=pt4x;if (max_x<pt5x) max_x=pt5x;if (max_x<pt6x) max_x=pt6x;if (max_x<pt7x) max_x=pt7x;
    int min_y=pt0y;if (min_y>pt1y) min_y=pt1y;if (min_y>pt2y) min_y=pt2y;if (min_y>pt3y) min_y=pt3y;
    if (min_y>pt4y) min_y=pt4y;if (min_y>pt5y) min_y=pt5y;if (min_y>pt6y) min_y=pt6y;if (min_y>pt7y) min_y=pt7y;
    int max_y=pt0y;if (max_y<pt1y) max_y=pt1y;if (max_y<pt2y) max_y=pt2y;if (max_y<pt3y) max_y=pt3y;
    if (max_y<pt4y) max_y=pt4y;if (max_y<pt5y) max_y=pt5y;if (max_y<pt6y) max_y=pt6y;if (max_y<pt7y) max_y=pt7y;

    //determine if face was to be drawn checking if the normal is looking in the camera direction (z>0)
    //face 1 normal = -vect z
    //face 2 normal = +vect z
    //face 3 normal = +vect x
    //face 4 normal = +vect y
    //face 5 normal = -vect x
    //face 6 normal = -vect y


    if (-vect_vox_z_c.z>0)
        quad(pt0x,pt0y,pt1x,pt1y,pt2x,pt2y,pt3x,pt3y,0x7F+0x80*-vect_vox_z_c.z/ORTHO_ZOOM);
    if (+vect_vox_z_c.z>0)
        quad(pt4x,pt4y,pt5x,pt5y,pt6x,pt6y,pt7x,pt7y,0x7F+0x80*+vect_vox_z_c.z/ORTHO_ZOOM);
    if (+vect_vox_x_c.z>0)
        quad(pt1x,pt1y,pt2x,pt2y,pt6x,pt6y,pt5x,pt5y,0x7F+0x80*+vect_vox_x_c.z/ORTHO_ZOOM);
    if (+vect_vox_y_c.z>0)
        quad(pt2x,pt2y,pt3x,pt3y,pt7x,pt7y,pt6x,pt6y,0x7F+0x80*+vect_vox_y_c.z/ORTHO_ZOOM);
    if (-vect_vox_x_c.z>0)
        quad(pt0x,pt0y,pt3x,pt3y,pt7x,pt7y,pt4x,pt4y,0x7F+0x80*-vect_vox_x_c.z/ORTHO_ZOOM);
    if (-vect_vox_y_c.z>0)
        quad(pt1x,pt1y,pt0x,pt0y,pt4x,pt4y,pt5x,pt5y,0x7F+0x80*-vect_vox_y_c.z/ORTHO_ZOOM);

    /*if (-vect_vox_z_c.z>0)
        quad(pt0x,pt0y,pt1x,pt1y,pt2x,pt2y,pt3x,pt3y,0xFFFFFFFF);
    if (+vect_vox_z_c.z>0)
        quad(pt4x,pt4y,pt5x,pt5y,pt6x,pt6y,pt7x,pt7y,0xFFFFFFFF);
    if (+vect_vox_x_c.z>0)
        quad(pt1x,pt1y,pt2x,pt2y,pt6x,pt6y,pt5x,pt5y,0xFFFFFFFF);
    if (+vect_vox_y_c.z>0)
        quad(pt2x,pt2y,pt3x,pt3y,pt7x,pt7y,pt6x,pt6y,0xFFFFFFFF);
    if (-vect_vox_x_c.z>0)
        quad(pt0x,pt0y,pt3x,pt3y,pt7x,pt7y,pt4x,pt4y,0xFFFFFFFF);
    if (-vect_vox_y_c.z>0)
        quad(pt1x,pt1y,pt0x,pt0y,pt4x,pt4y,pt5x,pt5y,0xFFFFFFFF);*/

    if (one_voxel_picture!=0)
      SDL_FreeSurface(one_voxel_picture);
    one_voxel_picture=zoomSurface (one_voxel_picture_big,1/oversampling , 1/oversampling,SMOOTHING_ON);
    texture1 = glsurf2Texture(one_voxel_picture,texture1);
}

void Graph_OGL::dessineRectangle(double x,double y, double largeur,double hauteur,unsigned short r,unsigned short g,unsigned short b,unsigned short a)
{
    glColor3ub(r,g,b); //la couleur peut etre donnee hors de glBegin, ouf
    glColor4ub(r,g,b,a); //la couleur peut etre donnee hors de glBegin, ouf
    
    //glBindTexture(GL_TEXTURE_2D, texture1);
    //glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glTexCoord2d(0,0);
    glVertex2d(x,y);
    glTexCoord2d(1,0);
    glVertex2d(x+largeur,y);
    glTexCoord2d(1,1);
    glVertex2d(x+largeur,y+hauteur);
    glTexCoord2d(0,1);
    glVertex2d(x,y+hauteur);
    //glPopMatrix();
}

void Graph_OGL::draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a)
{
    dessineRectangle((LARGEUR_FENETRE>>1)-(picture_size>>1)+x,(HAUTEUR_FENETRE>>1)-(picture_size>>1)+y,picture_size,picture_size,r,g,b,a);
}
    
