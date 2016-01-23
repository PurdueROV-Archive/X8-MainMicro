#ifndef
#define THRUST_MAPPER

#include "matrices.h"


typedef struct {

	MapperMatrixIndex currentMapperMatrix = ALL;

	matrix8_6 matrices[] = {
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
		void calculateThrustMap(vect6 target_vector);
		void changeMapperMatrix(int* enabled_thrusters);
		vect6 getCurrentForceVector(void);
		void nullForceVector(void);
		vect8 thrust_map;

	private:
		vect6 desired_force_vector;
		vect3 pivotPosition;
		ThrustMapperMatrices mapper_matrices;

};


#endif