#include "matricies.h"

int dot(vect6 a, vect6 b)
{
    return a.L.x * b.L.x + a.L.y * b.L.y + a.L.z * b.L.z + a.R.x * b.R.x + a.R.y * b.R.y + a.R.z * b.R.z;  
}

int dot(vect3 a, vect3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vect3 cross(vect3 a, vect3 b)
{
    vect3 result;
    result.x = 
    result.y = 
    result.z = 
}