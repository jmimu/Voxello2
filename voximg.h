#ifndef VOXIMG_H
#define VOXIMG_H

#include <string>
#include "pt3d.h"

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

//define directions
#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3


#define VOX_FILE 1

class VoxImg
{
public:
    VoxImg();
    VoxImg(std::string filnam,int direction=EAST,int file_format=VOX_FILE);
    virtual ~VoxImg();
    //load form voxlap file
    bool load_from_VOX (std::string filnam,int direction=EAST);///load Ken Silverman's Voxlap5 VOX format
    unsigned char *voxels;
    unsigned char palette[256][3];

    unsigned short get_xyzsiz(){return xyzsiz;}
    unsigned short get_yzsiz(){return yzsiz;}
    unsigned short get_xsiz(){return xsiz;}
    unsigned short get_ysiz(){return ysiz;}
    unsigned short get_zsiz(){return zsiz;}
public:
    unsigned short xsiz, ysiz, zsiz;
    
    long xyzsiz;//=xsiz*ysiz*zsiz;
    long yzsiz;//=ysiz*zsiz;
};

#endif // VOXIMG_H
