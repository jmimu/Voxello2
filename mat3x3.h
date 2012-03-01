//      mat3x3.h
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


#ifndef MAT3X3_H
#define MAT3X3_H

#include <iostream>
#include <vector>
#include <math.h>

class Mat3x3
{
    public:
        Mat3x3();
        double matr[3][3];
        
        //static car cree juste une matrice rotation, donc ne depend pas d'un objet Mat3x3
        static Mat3x3 matrice_rot(double angle_X,double angle_Y,double angle_Z);
        
        Mat3x3 mult(Mat3x3 B);
};

#endif /* MAT3X3_H */ 
