// INCLUDES:
#include "overseer.h"


// CLASS FUNCTIONS:

// CONSTRUCTOR:
Overseer::Overseer(void)
{
	int i;
	flag_NewData = NEW_DATA;
	for (i = 0; i < 8; i++)
		thrusters.powers[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.currents[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.voltages[i] = 0;
	for (i = 0; i < 8; i++)
		thrusters.enabled[i] = 0;
}


// Gives new data to Overseer, and sets NEW_DATA_FLAG to NEW_DATA
//
// @note - Run in the interrupt. Does no calculations.
void Overseer::update(vect6 force, vect3 pivotPos, char on_off)
{
	target_force = force;
	thrustMapper.adjustPivotPosition(pivotPos);
	thrustMapper.changeMapperMatrix(on_off);

	flag_NewData = NEW_DATA;
}

// Checks if new data has been logged (NEW_DATA_FLAG set), calculates if true.
//
// @returns - NEW_DATA_FLAG.
// @note - Run in main while loop (not during interrupt).
int Overseer::checkForUpdate(void)
{
	if (flag_NewData)
		calculateAndPush();
	return flag_NewData;
}

// Calculates the thrust for each motor, and pushes to the motors.
void Overseer::calculateAndPush(void)
{
	thrustMapper.calculateThrustMap(target_force);
	// send the thrustMapper.thrust_map to the motors (thrusters) here:
}


// Will update information about the motors.
void Overseer::updateFromThrusters(void)
{
	// Update Current
	// Update Voltage
}