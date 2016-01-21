#include "matricies.h"



void transpose(vect6 vec)
{
    vec.transposed = !vec.transposed;
}

void transpose(vect3 vec)
{
    vec.transposed = !vec.transposed;
}

void transpose(vect6 vec)
{
    vec.transposed = !vec.transposed;
}

int dot(vect6 a, vect6 b)
{
    if (a.transposed == b.transposed)
        return a.Lx * b.Lx + a.Ly * b.Ly + a.Lz * b.Lz + a.Rx * b.Rx + a.Ry * b.Ry + a.Rz * b.Rz;
    else
        throw 1;    
}

vect3 cross(vect3 a, vect3 b)
{
    vect3 result;
    result.x = 
    result.y = 
    result.z = 
}