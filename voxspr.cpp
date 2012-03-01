#include "voxspr.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>



VoxSpr::VoxSpr() :
        m_img(NULL),m_pos(0,0,0)
{
}

VoxSpr::VoxSpr(VoxImg * img) :
        m_img(img),m_pos()
{
}

VoxSpr::~VoxSpr()
{

}
