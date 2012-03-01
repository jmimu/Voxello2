#ifndef VOXSPR_H
#define VOXSPR_H

#include <string>
#include "pt3d.h"
#include "voximg.h"

/**
  Object's frame of reference

      / North (+y)
     /
    /
   /_ _ _ _ East (+x)
  |
  |
  |
  Down (+z)
  */


#define VOX_FILE 1

class VoxSpr
{
public:
    VoxSpr();
    VoxSpr(VoxImg * img);
    virtual ~VoxSpr();

    void set_pos(Pt3d &pt){m_pos=pt;}
    
    VoxImg * get_img(){return m_img;};
    void set_img(VoxImg * img){m_img=img;};
    Pt3d * get_pos(){return &m_pos;};
    
    unsigned short get_xyzsiz(){return m_img->get_xyzsiz();}
    unsigned short get_yzsiz(){return m_img->get_yzsiz();}
    unsigned short get_xsiz(){return m_img->get_xsiz();}
    unsigned short get_ysiz(){return m_img->get_ysiz();}
    unsigned short get_zsiz(){return m_img->get_zsiz();}
public:
    VoxImg * m_img;
    Pt3d m_pos;
};

#endif // VOXSPR_H
