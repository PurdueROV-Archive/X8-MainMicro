#include "matrices.h"

int dot(vect6 a, vect6 b)
{
    //return a.L.x * b.L.x + a.L.y * b.L.y + a.L.z * b.L.z + a.R.x * b.R.x + a.R.y * b.R.y + a.R.z * b.R.z; 
    return dot(a.L,b.L) + dot(a.R,b.R);
}

int dot(vect3 a, vect3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vect3 cross(vect3 a, vect3 b)
{
    vect3 result;
    
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

vect3 add(vect3 a, vect3 b)
{
    vect3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vect6 add6(vect6 a, vect6 b)
{
    vect6 result;
    result.L = add(a.L,b.L);
    result.R = add(a.R,b.R);
    return result;
}

vect3 sub(vect3 a, vect3 b)
{
    vect3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vect6 sub6(vect6 a, vect6 b)
{
    vect6 result;
    result.L = sub(a.L,b.L);
    result.R = sub(a.R,b.R);
    return result;
}

vect3 mul(vect3 a, int factor)
{
    vect3 result;
    result.x = a.x * factor;
    result.y = a.y * factor;
    result.z = a.z * factor;
    return result;
}

vect6 mul6(vect6 a, int factor)
{
    vect6 result;
    result.L = mul(a.L, factor);
    result.R = mul(a.R, factor);
    return result;
}

vect3 div(vect3 a, int factor)
{
    vect3 result;
    result.x = a.x / factor;
    result.y = a.y / factor;
    result.z = a.z / factor;
    return result;
}

vect6 div6(vect6 a, int factor)
{
    vect6 result;
    result.L = div(a.L, factor);
    result.R = div(a.R, factor);
    return result;
}

vect3 vect3Make(int a, int b, int c)
{
    vect3 result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}

vect6 vect6Make(int a, int b, int c, int d, int e, int f)
{
    vect6 result;
    result.L = vect3Make(a, b, c);
    result.R = vect3Make(d, e, f);
    return result;
}

vect8 matMul(matrix8_6 mat, vect6 v)
{
    vect8 result;
    result.a = dot(mat.t1, v);
    result.b = dot(mat.t2, v);
    result.c = dot(mat.t3, v);
    result.d = dot(mat.t4, v);
    result.e = dot(mat.t5, v);
    result.f = dot(mat.t6, v);
    result.g = dot(mat.t7, v);
    result.h = dot(mat.t8, v);
    return result;
}