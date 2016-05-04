#ifndef OVERSEER
#define OVERSEER


#include <stdint.h>
#include "matrices.h"
#include "thrust_mapper.h"

#define ESC_ADDRESS_0 0x29


// STRUCTS:

// May be deprecated when the Thruster class is written.
typedef struct {

	int powers[8];
	int currents[8];
	int voltages[8];
	char enabled[8];

} ThrustersContainer;

// DEFINES:

// Used by the public int flag_NewData.
#define NO_NEW_DATA 0
#define NEW_DATA 1

#define THRUST_MAX 32767.0
#define FLOATPT_TO_INT_SCALE 10000


// OVERSEER CLASS

/*
 *	How to Use the Overseer
 *
 *	1. Create new Overseer object.
 *
 *	2. When new data arrives from Control during an interrupt (new force vector, etc.) call update().
 *
 *	3. Setup timer to, at certain intervals, perform ramping via doRamping().
 *
 *	4. On the main thread, check whether update() has been called, via checkForUpdate().
 *		This will call calculateAndPush() if there is new data to be sent to thrust_mapper.
 *
 *	5. When data comes in from the thrusters' interrupts, update the thruster data via updateFromThrusters().
 */

 // TODO: May need to make some variables volatile.

class Overseer
{
	public:
        Overseer();
        void update(vect6 force, vect3 pivotPos, uint8_t on_off);
        int checkForUpdate(void);
        void calculateAndPush(void);
        void updateFromThrusters(void);
        void scaleOverflow(vect8 * thrust_map, int32_t max);
        int16_t* getThrusters(void);
        void doRamping(void);
        int flag_NewData;
        int is_Overflowing;
        void sendToMotors(void);

        // DEBUG METHODS:
        vect8 getThrust_Map(void);
        ThrustMapper getThrustMapper(void);
        vect6 getTargetForce(void);
        int areOverseerAndMapperCommunicating(void);

	private:
        ThrustersContainer thrusters;
        ThrustMapper thrustMapper;
        vect6 target_force;
        vect6 Tset;
        vect6 Tcurrent;
        //Arduino_I2C_ESC motor(0x29); //[8];
        vect8 currentDeliveredThrust;
        int usePWM;
        int16_t* thrusterValues;
};

#endif
