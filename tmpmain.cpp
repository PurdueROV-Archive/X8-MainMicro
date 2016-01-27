#include <stdio.h>
#include "matrices.h"

void printVector(vect3);
void printVector6(vect6);
void printMatrix8_6(matrix8_6);
void printVector8(vect8);
void printMatrix2_2(matrix2_2);

matrix8_6 pivM(void);

int main(void)
{
//    printf("Vector cross and add test:\n");
//	  vect3 a = vect3Make(1,2,3);
//    vect3 b = vect3Make(4,5,6);
//    printf("a cross b: \n");
//    vect3 c = cross(a,b);
//    printVector(c);
//    printf("a add b: \n");
//    vect3 d = add(a,b);
//    printVector(d);
    
//    printf("MatMul 86x61 test:\n");
//    vect6 F = vect6Make(1,1,0,1,0,1);
//    matrix8_6 m = pivM();
//    vect8 thrusts = matMul_86x61(m, F);
//    printVector8(thrusts);
//    printf("\n");

    printf("MatMul 22x22 test:\n");
    matrix2_2 a, b;
    a.one = vect2Make(1,2);
    a.two = vect2Make(3,4);
    
    b.one = vect2Make(1,2);
    b.two = vect2Make(3,4);
    
    matrix2_2 c = matMul_22x22(a,b);
    
    matrix2_2 invC = invert2_2(c);
    matrix2_2 d = matMul_22x22(c,invC);
    printMatrix2_2(c);
    printMatrix2_2(invC);
    printMatrix2_2(d);
    
    return 0;
}

void printMatrix2_2(matrix2_2 m)
{
    printf("Matrix 2x2:\n%5d %5d\n%5d %5d\n",m.one.a, m.one.b, m.two.a, m.two.b);
}

void printVector(vect3 v)
{
    printf("x = %5d, y = %5d, z = %5d",v.x,v.y,v.z);
}

void printVector6(vect6 v)
{
    printf("L:\t");
    printVector(v.L);
    printf(",\tR:\t");
    printVector(v.R);
}

void printVector8(vect8 v)
{
    printf("a = %5d, b = %5d, c = %5d, d = %5d, e = %5d, f = %5d, g = %5d, h = %5d\n",
           v.a, v.b, v.c, v.d, v.e, v.f, v.g, v.h);
}

void printMatrix8_6(matrix8_6 m)
{
    printVector6(m.t1);
    printf("\n");
    printVector6(m.t2);
    printf("\n");
    printVector6(m.t3);
    printf("\n");
    printVector6(m.t4);
    printf("\n");
    printVector6(m.t5);
    printf("\n");
    printVector6(m.t6);
    printf("\n");
    printVector6(m.t7);
    printf("\n");
    printVector6(m.t8);
    printf("\n");
}

matrix8_6 pivM()
{
    matrix8_6 result;
    result.t1 = vect6Make(0, 0, 250, -100,  250, 0);
    result.t2 = vect6Make(0, 0, 250, -100, -250, 0);
    result.t3 = vect6Make(0, 0, 250,  100,  250, 0);
    result.t4 = vect6Make(0, 0, 250,  100, -250, 0);
    
    result.t5 = vect6Make(-750,  375, 0, 0, 0, -125);
    result.t6 = vect6Make(-750, -375, 0, 0, 0,  125);
    result.t7 = vect6Make( 750,  375, 0, 0, 0,  125);
    result.t8 = vect6Make( 750, -375, 0, 0, 0, -125);
    return result;
}