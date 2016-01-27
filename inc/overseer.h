
#ifndef OVERSEER
#define OVERSEER

// INCLUDES:
#include <stdint.h>
#include "matrices.h"
#include "thrust_mapper.h"


// STRUCTS:

typedef struct {

	int powers[8];
	int currents[8];
	int voltages[8];
	// Contains 1/0 depending on whether each motor 1-8 is 'enabled' or 'not disabled.'
	uint8_t enabled[8];

} ThrustersContainer;

// OVERSEER CLASS

class Overseer
{
	public:
		Overseer();
		void update(vect6 force, vect3 pivotPos, char on_off);
		void updateFromThrusters(void);

	private:
		ThrustersContainer thrusters;
		ThrustMapper thrustMapper;

};

#endif