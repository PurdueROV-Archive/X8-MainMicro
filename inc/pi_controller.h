#ifndef PI_CONTROLLER
#define PI_CONTROLLER

// INCLUDES:

#include "matrices.h"
#include <math.h>
#include <stdint.h>

// STRUCTS:

typedef struct {

	// Proportional Constant = controller gain
	float P_rot;
	float P_Z;
	// Integral Constant = controller gain / reset time
	float I_rot;
	float I_Z;



} PIDConstants;

typedef struct {

	// rotation position to keep ROV at (stabilize ROV at this rotation).
	vect3 rot_ref;
	vect3 rot_est;
	// rotation position
	vect3 rot_error;
	// /_\ (delta) rotation velocity, isn't used at the moment
	// vect3 rot_est_vel;
	
	// Z depth reference and error
	float Z_ref;
	float Z_error;

	// Last Force vector calculated from the PID controller (from the last time getOutput was called).
	vect3 lastForce;
	float Z_lastForce;
	// Integral sum
	vect3 integralSum;
	float Z_integralSum;

	uint32_t lastTime;
	uint32_t timeDiff;

	vect3 CObias;

} PIDInputData;

// DEFINES:
#ifndef ON
#define ON 1
#endif
#ifndef OFF
#define OFF 0
#endif

#define P_INIT_VALUE 0
#define I_INIT_VALUE 0
#define INTEGRAL_RECORDING_SIZE 100


// CLASS:

class PIController
{

	public:

		PIController(void);
		void stop(void);
		void start(void);
		void set_ref(vect3 rot_ref, float Z_ref);
		void updateRotation(vect3 rot_ref);
		void set_PI(float newP_R, float newI_R, float newP_Z, float newI_Z, uint8_t state);
		void sensorInput(vect3 rot_est, float Z_est, int32_t timems);
		vect6 getOutput(vect6 inputThrust);

	private:

		uint8_t ON_OFF;
		PIDConstants consts;
		PIDInputData data;

};

#endif
