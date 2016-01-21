#ifndef
#define THRUST_MAPPER

#include "matrices.h"


typedef struct {

	// t1_l = thruster1_location = (x,y,z) dist (m) from center of mass.
	vect3 t1;
	vect3 t2;
	vect3 t3;
	vect3 t4;
	vect3 t5;
	vect3 t6;
	vect3 t7;
	vect3 t8;

} ThrusterLocationData;

typedef struct {

	// t1_r = thruster1_rotation = (x-axis-angle, y-axis-angle, z-axis-angle) degrees, from...?
	vect3 t1;
	vect3 t2;
	vect3 t3;
	vect3 t4;
	vect3 t5;
	vect3 t6;
	vect3 t7;
	vect3 t8;

} ThrusterRotationData;

typedef struct {

	MapperMatrixIndex currentMapperMatrix = ALL;

	matrix8_6 mapperMatrices[] = {
		matrix8_6 all,
		matrix8_6 minus_t1,
		matrix8_6 minus_t2,
		matrix8_6 minus_t3,
		matrix8_6 minus_t4,
		matrix8_6 minus_t5,
		matrix8_6 minus_t6,
		matrix8_6 minus_t7,
		matrix8_6 minus_t8
	};

} ThrustMapperMatrices;


enum MapperMatrixIndex {
	ALL = 0,
	T1,
	T2,
	T3,
	T4,
	T5,
	T6,
	T7,
	T8
}


class ThrustMapper
{	
	public:
		ThrustMapper(void);
		matrix8_3 calculateThrustMap(vect6 target_vector);
		void changeMapperMatrix(int* enabled_thrusters);
		vect6 desired_force_vector;
		matrix8_3 thrust_map; //??? Not sure what size the resultant thrustmap should look like.

	private:
		ThrusterLocationData locations;
		ThrusterRotationData rotations;
		ThrustMapperMatrices mapper_matrices;

};


#endif