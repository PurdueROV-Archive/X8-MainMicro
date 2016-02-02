#include "pid.h"

// CLASS METHODS:

PIDController::PIDController(void)
{

}

void PIDController::stop(void)
{
	ON_OFF = OFF;
}

// Call every time the PI Controller will be started or restarted.
// This method ensures that the 
void PIDController::start(void)
{
	data.rot_ref = data.rot_est;
	data.rot_est = vect3Make(0,0,0);
	data.rot_est_vel = vect3Make(0,0,0);
	data.CObias = lastForce;
	data.integralSum = 0;
	ON_OFF = ON;
}

// Sets new target rotation orientation.
// Perform this method even when PI is off.
void PIDController::setNewRotation(vect3 rot_ref)
{
	data.rot_ref = rot_ref;
}

// Sets the fusion data from the sensors to the data structure.
// Perform this method even when PI is off.
void PIDController::sensorInput(vect3 rot_est, vect3 rot_est_vel)
{
	data.rot_est = rot_est;
	data.rot_est_vel = rot_est_vel;
}

// Computes the resultant force vector to achieve the desired rotational orientation using the PI Controller.
// 
// @returns - the force vector computed from the PI Controller, or just the incoming force vector set by
//			Control.
vect3 getOutput(void)
{
	if (data.ON_OFF == OFF)
		return data.rot_ref;
	vect3 force;

	force = consts.CObias + consts.controllerGain * (data.rot_ref - data.rot_est) + (consts.controllerGain / resetTime) * 

	return force;
}