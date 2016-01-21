/* Put all of your #includes and #defines up here   */

#include "thrust_mapper.h"


/*Put all function definitions here. Do not forget the 
  classname:: part before the function name! */

/* Second note, each class must have a constructor. This
   is a function that is called when you create the object 
   (just like creating a variable) of the class. This function
   will have no return type. */


ThrustMapper::ThrustMapper(void)
{
	/*
	 *	Initialize thruster location data here
	 */

	/*
	 *	Initialize thruster rotation data here
	 */

	/*
	 *	Initialize mapper matrices here
	 */


}

matrix8_3 ThrustMapper::calculateThrustMap(vect6 target_vector)
{
	desired_force_vector = target_vector;

	// Calculations:

	return thrust_map;
}

// Only accepts an array of 8 ints. These ints correspond to whether each of the 8 thrusters are enabled.
void ThrustMapper::changeMapperMatrix(MapperMatrixIndex matindex)
{
	mapper_matrices.currentMapperMatrix = matindex;
}

