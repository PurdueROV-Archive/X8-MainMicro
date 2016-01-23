#include <stdio.h>
#include "matrices.h"

void printVector(vect3);
void printVector6(vect6);
matrix8_6 pivM(void);

int main(void)
{
//	vect3 a = vect3Make(1,2,3);
//    vect3 b = vect3Make(4,5,6);
//    printf("a cross b: \n");
//    vect3 c = cross(a,b);
//    printVector(c);
//    printf("a add b: \n");
//    vect3 d = add(a,b);
//    printVector(d);
    
    printf("MatMul test:");
    
    vect6 F = vect6Make(1,1,0,1,0,1);
    vect8 thrusts = matMul(pivM(),F);
    
    int i = 0;
    char *data = (char*) &thrusts;
    for (i = 0; i < 8; i ++) {
        printf("%d\t",(*data));
        data++;
    }
    printf("\n");
    
    return 0;
}

void printVector(vect3 v)
{
    printf("x = %d, y = %d, z = %d\n",v.x,v.y,v.z);
}

void printVector6(vect6 v)
{
    printf("L:\t");
    printVector(v.L);
    printf("R:\t");
    printVector(v.R);
}

matrix8_6 pivM()
{
    matrix8_6 result;
    result.t1 = vect6Make(0, 0, 250, -100,  250, 0);
    result.t2 = vect6Make(0, 0, 250, -100, -250, 0);
    result.t3 = vect6Make(0, 0, 250,  100,  250, 0);
    result.t4 = vect6Make(0, 0, 250,  100, -250, 0);
    
    result.t5 = vect6Make(-750,  375, 0, 0, 0, -125);
    result.t5 = vect6Make(-750, -375, 0, 0, 0,  125);
    result.t5 = vect6Make( 750,  375, 0, 0, 0,  125);
    result.t5 = vect6Make( 750, -375, 0, 0, 0, -125);
    return result;
}