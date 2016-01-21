#include "matricies.h"

typedef struct{
    int Lx;
    int Ly;
    int Ly;
    int Rx;
    int Ry;
    int Rz;
    bool transposed;
}vect6;

typedef struct{
    vect6 t1;
    vect6 t2;
    vect6 t3;
    vect6 t4;
    vect6 t5;
    vect6 t6;
    vect6 t7;
    vect6 t8;
    bool transposed;
}matrix8_6;

typedef struct{
    int x;
    int y;
    int z;
    bool transposed;
}vect3;

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
    if (a.transposed == b.transposed) {
        return a.Lx * b.Lx + a.Ly * b.Ly + a.Lz * b.Lz + a.Rx * b.Rx + a.Ry * b.Ry + 
        
}