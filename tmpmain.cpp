#include <stdio.h>
#include "matrices.h"

int main(void)
{
	vect3 a = vect3Make(1,2,3);
    vect3 b = vect3Make(4,5,6);
    vect3 c = cross(a,b);
    printf("x = %d, y = %d, z = %d\n",c.x,c.y,c.z);
    return 0;
}