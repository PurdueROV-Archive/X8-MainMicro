typedef struct{
    int x;
    int y;
    int z;
}vect3;

typedef struct{
    vect3 L;
    vect3 R;
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
    vect3 t1;
    vect3 t2;
    vect3 t3;
    vect3 t4;
    vect3 t5;
    vect3 t6;
    vect3 t7;
    vect3 t8;
}matrix8_3;

vect3 cross(vect3, vect3);
int dot(vect3, vect3);

