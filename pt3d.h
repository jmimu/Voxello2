//      Pt3d.h
//      
//      Copyright 2011 Roa <roa@idefix>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      


#ifndef PT3D_H
#define PT3D_H

#include <math.h>
#include "mat3x3.h"

inline int sign(double x)
{
    if (x>0.1) return 1;
    if (x<-0.1) return -1;
    return 0;
}




class Pt2d
{
    public:
        Pt2d(double _l=0.0,double _c=0.0);
        double x,y;
};


class Pt3d: public Pt2d
{
    public:
        Pt3d(double _x=0.0,double _y=0.0,double _z=0.0);
        Pt3d mult(Mat3x3 M);
        void copy(Pt3d &p);
        void scal_mult(float f);
        void mult(Mat3x3 M,Pt3d *out);//in place
        void mult_proj(Mat3x3 M,Pt2d *out);//computes only x' and y'
        void add(Pt3d M,Pt3d *out);//out = this+M
        void sub(Pt3d M,Pt3d *out);//out = this-M
        void add(Pt3d M);//this = this+M
        void sub(Pt3d M);//this = this-M
        
        double z;
};

#endif /* PT3D_H */ 
