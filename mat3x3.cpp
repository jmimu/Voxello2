//      mat3x3.cpp
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


#include "mat3x3.h"

Mat3x3::Mat3x3()
{
//    matr={{0,0,0},{0,0,0},{0,0,0}};
}
        
//static car cree juste une matrice rotation, donc ne depend pas d'un objet Mat3x3
Mat3x3 Mat3x3::matrice_rot(double angle_X,double angle_Y,double angle_Z)
{
    Mat3x3 Rx;
    Rx.matr[0][0]= 1; Rx.matr[0][1]=            0; Rx.matr[0][2]=             0;
    Rx.matr[1][0]= 0; Rx.matr[1][1]= cos(angle_X); Rx.matr[1][2]=-sin(angle_X);
    Rx.matr[2][0]= 0; Rx.matr[2][1]= sin(angle_X); Rx.matr[2][2]= cos(angle_X);
    Mat3x3 Ry;
    Ry.matr[0][0]= cos(angle_Y); Ry.matr[0][1]= 0; Ry.matr[0][2]=sin(angle_Y);
    Ry.matr[1][0]=            0; Ry.matr[1][1]= 1; Ry.matr[1][2]=            0;
    Ry.matr[2][0]=-sin(angle_Y); Ry.matr[2][1]= 0; Ry.matr[2][2]= cos(angle_Y);
    Mat3x3 Rz;
    Rz.matr[0][0]= cos(angle_Z); Rz.matr[0][1]=-sin(angle_Z); Rz.matr[0][2]=0;
    Rz.matr[1][0]= sin(angle_Z); Rz.matr[1][1]= cos(angle_Z); Rz.matr[1][2]=0;
    Rz.matr[2][0]=            0; Rz.matr[2][1]=            0; Rz.matr[2][2]=1;
    
    Mat3x3 RxRy=Rx.mult(Ry);
    
    return RxRy.mult(Rz);
}

Mat3x3 Mat3x3::mult(Mat3x3 B)
{
    Mat3x3 C;
    for (unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        { 
            C.matr[i][j]=0;
            for (unsigned int k=0; k<3; k++)
            {
                C.matr[i][j]+=this->matr[i][k]*B.matr[k][j];
            }
        }
    }
    return C;
}


