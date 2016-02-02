#ifndef PID_CONTROLLER
#define PID_CONTROLLER

// INCLUDES:

#include <math.h>

// STRUCTS:

typedef struct {

	const float controllerGain = 0;
	const float resetTime = 0;

} PIDConstants;

typedef struct {

	// rotation position to keep ROV at (stabilize ROV at this rotation).
	vect3 rot_ref;
	// rotation position
	vect3 rot_est;
	// /_\ (delta) rotation velocity
	vect3 rot_est_vel;
	// Last Force vector calculated from the PID controller (from the last time getOutput was called).
	vect3 lastForce;
	// Integral data & sum
	float integralData[INTEGRAL_RECORDING_SIZE];
	float integralSum;

	float CObias = 0;

} PIDInputData;

// DEFINES:
#ifndef ON
#define ON 1
#endif
#ifndef OFF
#define OFF 0
#endif

#define INTEGRAL_RECORDING_SIZE 100

// CLASS:

class PIDController
{

	public:

		PIDController(void);
		void stop(void);
		void start(void);
		void setNewRotation(vect3 rot_ref);
		void sensorInput(vect3 rot_est, vect3 rot_est_vel);
		vect3 getOutput(void);

	private:

		int ON_OFF = ON;
		PIDConstants consts;
		PIDInputData data;

};

#endif