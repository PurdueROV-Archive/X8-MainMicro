// INCLUDES:
#include "overseer.h"


// CLASS FUNCTIONS:

Overseer::Overseer(void)
{
	int i;
	for (i = 0; i < 8; i++)
		thrusters.powers[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.currents[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.voltages[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.enabled[i] = 0;
}



void Overseer::update(vect6 force, vect3 pivotPos, char on_off)
{
	thrustMapper.adjustPivotPosition(pivotPos);
	thrustMapper.changeMapperMatrix(on_off);
	thrustMapper.calculateThrustMap(force);
}



void Overseer::updateFromThrusters(void)
{

}