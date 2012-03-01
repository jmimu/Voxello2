//      Pt3d.cpp
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


#include "pt3d.h"

Pt2d::Pt2d(double _l,double _c)
{
    x=_l;y=_c;
}

void Pt3d::add(Pt3d M){this->add(M,this);}

void Pt3d::add(Pt3d M,Pt3d *out)//out = this+M
{
    out->x=this->x+M.x;
    out->y=this->y+M.y;
    out->z=this->z+M.z;
}

void Pt3d::sub(Pt3d M){this->sub(M,this);}

void Pt3d::sub(Pt3d M,Pt3d *out)//out = this-M
{
    out->x=this->x-M.x;
    out->y=this->y-M.y;
    out->z=this->z-M.z;
}

void Pt3d::scal_mult(float f)
{
    this->x*=f;
    this->y*=f;
    this->z*=f;
}



Pt3d::Pt3d(double _x,double _y,double _z)
{
    x=_x;y=_y;z=_z;
}

void Pt3d::copy(Pt3d &p)
{
    x=p.x;y=p.y;z=p.z;
}

Pt3d Pt3d::mult(Mat3x3 M)
{
    Pt3d p(0,0,0);
    //p = M*this
    p.x=M.matr[0][0]*this->x+M.matr[0][1]*this->y+M.matr[0][2]*this->z;
    p.y=M.matr[1][0]*this->x+M.matr[1][1]*this->y+M.matr[1][2]*this->z;
    p.z=M.matr[2][0]*this->x+M.matr[2][1]*this->y+M.matr[2][2]*this->z;
    return p;
}

void Pt3d::mult(Mat3x3 M,Pt3d *out)//in place
{
    //p = M*this
    out->x=M.matr[0][0]*this->x+M.matr[0][1]*this->y+M.matr[0][2]*this->z;
    out->y=M.matr[1][0]*this->x+M.matr[1][1]*this->y+M.matr[1][2]*this->z;
    out->z=M.matr[2][0]*this->x+M.matr[2][1]*this->y+M.matr[2][2]*this->z;
}

void Pt3d::mult_proj(Mat3x3 M,Pt2d *out)//in place
{
    //p = M*this
    out->x=M.matr[0][0]*this->x+M.matr[0][1]*this->y+M.matr[0][2]*this->z;
    out->y=M.matr[1][0]*this->x+M.matr[1][1]*this->y+M.matr[1][2]*this->z;
}

