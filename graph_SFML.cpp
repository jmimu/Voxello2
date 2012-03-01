#include "graph_SFML.h"


#include <iostream>


Graph_SFML::Graph_SFML():Graph(),App(),one_voxel_picture(),sprite_voxel(),surface_coverage()
{
    ORTHO_ZOOM=6.0;
}

void Graph_SFML::init()
{
    App.Create(sf::VideoMode(256*4, 192*4, 8), "SFML Window");
    one_voxel_picture.Create(SUB_PICTURE_SIZE,SUB_PICTURE_SIZE);

    sprite_voxel.SetTexture(one_voxel_picture.GetTexture());
}

void Graph_SFML::start_frame()
{
    App.Clear();
    //one_voxel_picture.Draw(sf::Shape::Rectangle(surface_coverage.Left,surface_coverage.Top,surface_coverage.Right,surface_coverage.Bottom, sf::Color(0,0,0));
    one_voxel_picture.Clear(sf::Color(0,0,0,0));
}

void Graph_SFML::end_frame()
{
    App.Display();
}

void Graph_SFML::line(int x1,int y1,int x2,int y2,unsigned short r,unsigned short g,unsigned short b)
{
    //lineColor(screen,x1,y1,x2,y2,color); 
    //App.Draw(sf::Shape::Line(x1, y1, x2, y2, 1, sf::Color::Red));
}

void Graph_SFML::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short v)
{
    quad(x1,y1,x2,y2,x3,y3,x4,y4,v,v,v);
}

void Graph_SFML::quad(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,unsigned short r,unsigned short g,unsigned short b)
{
    sf::ConvexShape Polygon(4);
    Polygon.SetPoint(0,sf::Vector2f(x1,SUB_PICTURE_SIZE-y1));
    Polygon.SetPoint(1,sf::Vector2f(x2,SUB_PICTURE_SIZE-y2));
    Polygon.SetPoint(2,sf::Vector2f(x3,SUB_PICTURE_SIZE-y3));
    Polygon.SetPoint(3,sf::Vector2f(x4,SUB_PICTURE_SIZE-y4));
    Polygon.SetFillColor(sf::Color(r,g,b));

    //Polygon.SetColor(sf::Color(255, 255, 255));
    //Polygon.EnableOutline(true);
    //Polygon.SetOutlineThickness(1);
    one_voxel_picture.Draw(Polygon);
}

void Graph_SFML::close()
{

}

void Graph_SFML::create_one_voxel_picture(Pt3d &M_000_c,Pt3d &vect_vox_x_c,Pt3d &vect_vox_y_c,Pt3d &vect_vox_z_c)
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

    //one_voxel_picture.Draw(sf::Shape::Rectangle(surface_coverage.Left,surface_coverage.Top,surface_coverage.Right,surface_coverage.Bottom, sf::Color(0,0,0));
    one_voxel_picture.Clear(sf::Color(0,0,0,0));

/*    float pt0x=M_000_c.x*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    float pt0y=M_000_c.y*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    float pt1x=pt0x+vect_vox_x_c.x+0*(sign(vect_vox_x_c.x)<<1);
    float pt1y=pt0y+vect_vox_x_c.y+0*(sign(vect_vox_x_c.y)<<1);
    float pt2x=pt1x+vect_vox_y_c.x+0*(sign(vect_vox_y_c.x)<<1);
    float pt2y=pt1y+vect_vox_y_c.y+0*(sign(vect_vox_y_c.y)<<1);
    float pt3x=pt2x-vect_vox_x_c.x-0*(sign(vect_vox_x_c.x)<<1);
    float pt3y=pt2y-vect_vox_x_c.y-0*(sign(vect_vox_x_c.y)<<1);
    float pt4x=pt0x+vect_vox_z_c.x+0*(sign(vect_vox_z_c.x)<<1);
    float pt4y=pt0y+vect_vox_z_c.y+0*(sign(vect_vox_z_c.y)<<1);
    float pt5x=pt1x+vect_vox_z_c.x+0*(sign(vect_vox_z_c.x)<<1);
    float pt5y=pt1y+vect_vox_z_c.y+0*(sign(vect_vox_z_c.y)<<1);
    float pt6x=pt2x+vect_vox_z_c.x+0*(sign(vect_vox_z_c.x)<<1);
    float pt6y=pt2y+vect_vox_z_c.y+0*(sign(vect_vox_z_c.y)<<1);
    float pt7x=pt3x+vect_vox_z_c.x+0*(sign(vect_vox_z_c.x)<<1);
    float pt7y=pt3y+vect_vox_z_c.y+0*(sign(vect_vox_z_c.y)<<1);*/

    float pt0x=M_000_c.x*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    float pt0y=M_000_c.y*ORTHO_ZOOM+(SUB_PICTURE_SIZE>>1) ;
    float pt1x=pt0x+add_one_in_dir(vect_vox_x_c.x);
    float pt1y=pt0y+add_one_in_dir(vect_vox_x_c.y);
    float pt2x=pt0x+add_one_in_dir(vect_vox_x_c.x+vect_vox_y_c.x);
    float pt2y=pt0y+add_one_in_dir(vect_vox_x_c.y+vect_vox_y_c.y);
    float pt3x=pt0x+add_one_in_dir(vect_vox_y_c.x);
    float pt3y=pt0y+add_one_in_dir(vect_vox_y_c.y);

    float pt4x=pt0x+add_one_in_dir(vect_vox_z_c.x);
    float pt4y=pt0y+add_one_in_dir(vect_vox_z_c.y);
    float pt5x=pt0x+add_one_in_dir(vect_vox_x_c.x+vect_vox_z_c.x);
    float pt5y=pt0y+add_one_in_dir(vect_vox_x_c.y+vect_vox_z_c.y);
    float pt6x=pt0x+add_one_in_dir(vect_vox_x_c.x+vect_vox_y_c.x+vect_vox_z_c.x);
    float pt6y=pt0y+add_one_in_dir(vect_vox_x_c.y+vect_vox_y_c.y+vect_vox_z_c.y);
    float pt7x=pt0x+add_one_in_dir(vect_vox_y_c.x+vect_vox_z_c.x);
    float pt7y=pt0y+add_one_in_dir(vect_vox_y_c.y+vect_vox_z_c.y);

 
    //compute min and max to minimize surface to blit
    int min_x=pt0x;if (min_x>pt1x) min_x=pt1x;if (min_x>pt2x) min_x=pt2x;if (min_x>pt3x) min_x=pt3x;
    if (min_x>pt4x) min_x=pt4x;if (min_x>pt5x) min_x=pt5x;if (min_x>pt6x) min_x=pt6x;if (min_x>pt7x) min_x=pt7x;
    int max_x=pt0x;if (max_x<pt1x) max_x=pt1x;if (max_x<pt2x) max_x=pt2x;if (max_x<pt3x) max_x=pt3x;
    if (max_x<pt4x) max_x=pt4x;if (max_x<pt5x) max_x=pt5x;if (max_x<pt6x) max_x=pt6x;if (max_x<pt7x) max_x=pt7x;
    int min_y=pt0y;if (min_y>pt1y) min_y=pt1y;if (min_y>pt2y) min_y=pt2y;if (min_y>pt3y) min_y=pt3y;
    if (min_y>pt4y) min_y=pt4y;if (min_y>pt5y) min_y=pt5y;if (min_y>pt6y) min_y=pt6y;if (min_y>pt7y) min_y=pt7y;
    int max_y=pt0y;if (max_y<pt1y) max_y=pt1y;if (max_y<pt2y) max_y=pt2y;if (max_y<pt3y) max_y=pt3y;
    if (max_y<pt4y) max_y=pt4y;if (max_y<pt5y) max_y=pt5y;if (max_y<pt6y) max_y=pt6y;if (max_y<pt7y) max_y=pt7y;


    //one_voxel_picture.Draw(sf::Shape::Rectangle(min_x,min_y, max_x-min_x, max_y-min_y,  sf::Color::Cyan));
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

    //blit only surface_coverage
    /*surface_coverage.Left=0;
    surface_coverage.Top=0;
    surface_coverage.Width=max_x;
    surface_coverage.Height=max_y;*/

    surface_coverage.Left=min_x;
    surface_coverage.Top=min_y;
    surface_coverage.Width=max_x-min_x;
    surface_coverage.Height=max_y-min_y;
    //std::cout<<surface_coverage.Left<<" "<<surface_coverage.Top<<" "<<surface_coverage.Width<<" "<<surface_coverage.Height<<"\n";
    //std::cout<<min_y<<" "<<max_y<<" "<<max_y-min_y;
    sprite_voxel.SetTexture(one_voxel_picture.GetTexture());
    sprite_voxel.SetTextureRect(surface_coverage);
    //one_voxel_picture.Draw(sf::Shape::Line(min_x,min_y, max_x, max_y, 1, sf::Color::Blue));
    //one_voxel_picture.Draw(sf::Shape::Line(max_x-2,max_y+2, max_x+2, max_y-2, 1, sf::Color::Green));
    //one_voxel_picture.Draw(sf::Shape::Line(min_x-2,min_y+2, min_x+2, min_y-2, 1, sf::Color::Blue));

    //std::cout<<"   "<<sprite_voxel.GetSubRect().Top<<" "<<sprite_voxel.GetSubRect().Height<<" "<<sprite_voxel.GetOrigin().y<<"\n";
}

void Graph_SFML::draw_1_voxel(int x,int y,unsigned short r,unsigned short g,unsigned short b,unsigned short a)
{
    int position_x=512-(SUB_PICTURE_SIZE>>1)+surface_coverage.Left+x;
    int position_y=384-(SUB_PICTURE_SIZE>>1)+surface_coverage.Top+y;

    sprite_voxel.SetPosition(position_x,position_y);
    sprite_voxel.SetColor(sf::Color(r,g,b,a));
    App.Draw(sprite_voxel);
}
    
