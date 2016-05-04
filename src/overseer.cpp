// INCLUDES:
#include "overseer.h"


// CLASS FUNCTIONS:

// CONSTRUCTOR:
Overseer::Overseer(void)
{
	int i;
	flag_NewData = NEW_DATA;
	for (i = 0; i < 8; i++)
  {
		thrusters.powers[i] = 0;
		thrusters.currents[i] = 0;
		thrusters.voltages[i] = 0;
		thrusters.enabled[i] = 0;
    //motors[i] = Arduino_I2C_ESC(ESC_ADDRESS_0+i);
    usePWM = 0;

  }
  
  
  /*Arduino_I2C_ESC* motor0 = new Arduino_I2C_ESC(0x29);
  Arduino_I2C_ESC* motor1 = new Arduino_I2C_ESC(0x2a);
  Arduino_I2C_ESC* motor2 = new Arduino_I2C_ESC(0x2b);
  Arduino_I2C_ESC* motor3 = new Arduino_I2C_ESC(0x2c);
  Arduino_I2C_ESC* motor4 = new Arduino_I2C_ESC(0x2d);
  Arduino_I2C_ESC* motor5 = new Arduino_I2C_ESC(0x2e);
  Arduino_I2C_ESC* motor6 = new Arduino_I2C_ESC(0x2f);
  Arduino_I2C_ESC* motor7 = new Arduino_I2C_ESC(0x30);
  motors[0] = motor0;
  motors[1] = motor1;
  motors[2] = motor2;
  motors[3] = motor3;
  motors[4] = motor4;
  motors[5] = motor5;
  motors[6] = motor6;
  motors[7] = motor7;*/

  //motor0.set(9);
  
  target_force = vect6Make(0,0,0,0,0,0);
  flag_NewData = 0;
  is_Overflowing = 0;
}


// Gives new data to Overseer, and sets NEW_DATA_FLAG to NEW_DATA
//
// @note - Run in the interrupt. Does no calculations.
void Overseer::update(vect6 force, vect3 pivotPos, uint8_t on_off)
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
	if (flag_NewData == NEW_DATA)
  {
    flag_NewData = NO_NEW_DATA;
		calculateAndPush();
  }
	return flag_NewData;
}

// Calculates the thrust for each motor, and pushes to the motors.
void Overseer::calculateAndPush(void)
{
	thrustMapper.calculateThrustMap(target_force);
    int max = 0;

    if ((max = max8(thrustMapper.thrust_map)) > THRUST_MAX) {
        scaleOverflow(&thrustMapper.thrust_map, max);
    } else {
        is_Overflowing = 0;
    }

	// send the thrustMapper.thrust_map to the motors (thrusters) here:
    sendToMotors();
}

void Overseer::sendToMotors(void)
{

  //Serial.print("before");
  //Arduino_I2C_ESC* motor0 = new Arduino_I2C_ESC(0x29);
  //motor0->set(4300);
  /*motors[1]->set(currentDeliveredThrust.b);
  motors[2]->set(currentDeliveredThrust.c);
  motors[3]->set(currentDeliveredThrust.d);
  motors[4]->set(currentDeliveredThrust.e);
  motors[5]->set(currentDeliveredThrust.f);
  motors[6]->set(currentDeliveredThrust.g);
  motors[7]->set(currentDeliveredThrust.h);*/
  //Serial.print("after");
/*  if (!usePWM)
  {
    motors[0].set(currentDeliveredThrust.a);
    motors[1].set(currentDeliveredThrust.b);
    motors[2].set(currentDeliveredThrust.c);
    motors[3].set(currentDeliveredThrust.d);
    motors[4].set(currentDeliveredThrust.e);
    motors[5].set(currentDeliveredThrust.f);
    motors[6].set(currentDeliveredThrust.g);
    motors[7].set(currentDeliveredThrust.h);
  }
  else
  {
    
  }
  */
   
    
    

  /*for(int i = 0; i < 8; i++)
  {
    //Serial.print("Loop");
    //motors[i]->update();
  }*/

}


// Scales all thrust values by the maximum overflowing thrust, keeping the same force vector.
void Overseer::scaleOverflow(vect8 * thrust_map, int32_t max)
{
    if (max < THRUST_MAX)
        return;

    is_Overflowing = 1;
    float scale = (float) THRUST_MAX / max;
    scale *= FLOATPT_TO_INT_SCALE;

    thrust_map->a *= scale;
    thrust_map->b *= scale;
    thrust_map->c *= scale;
    thrust_map->d *= scale;
    thrust_map->e *= scale;
    thrust_map->f *= scale;
    thrust_map->g *= scale;
    thrust_map->h *= scale;

    thrust_map->a /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->b /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->c /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->d /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->e /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->f /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->g /= (int32_t)FLOATPT_TO_INT_SCALE;
    thrust_map->h /= (int32_t)FLOATPT_TO_INT_SCALE;
}


// Will update information about the motors.
void Overseer::updateFromThrusters(void)
{
	// Update Current
	// Update Voltage
}

void Overseer::doRamping(void)
{
    if (currentDeliveredThrust.a < thrustMapper.thrust_map.a) {
      currentDeliveredThrust.a += MIN(1024, thrustMapper.thrust_map.a - currentDeliveredThrust.a);
    } else {
      currentDeliveredThrust.a += MAX(1024, thrustMapper.thrust_map.a - currentDeliveredThrust.a);
    }
    
    if (currentDeliveredThrust.b < thrustMapper.thrust_map.b) {
      currentDeliveredThrust.b += MIN(1024, thrustMapper.thrust_map.b - currentDeliveredThrust.b);
    } else {
      currentDeliveredThrust.b += MAX(1024, thrustMapper.thrust_map.b - currentDeliveredThrust.b);
    }
    
    if (currentDeliveredThrust.c < thrustMapper.thrust_map.c) {
      currentDeliveredThrust.c += MIN(1024, thrustMapper.thrust_map.c - currentDeliveredThrust.c);
    } else {
      currentDeliveredThrust.c += MAX(1024, thrustMapper.thrust_map.c - currentDeliveredThrust.c);
    }
    
    if (currentDeliveredThrust.d < thrustMapper.thrust_map.d) {
      currentDeliveredThrust.d += MIN(1024, thrustMapper.thrust_map.d - currentDeliveredThrust.d);
    } else {
      currentDeliveredThrust.d += MAX(1024, thrustMapper.thrust_map.d - currentDeliveredThrust.d);
    }
    
    if (currentDeliveredThrust.e < thrustMapper.thrust_map.e) {
      currentDeliveredThrust.e += MIN(1024, thrustMapper.thrust_map.e - currentDeliveredThrust.e);
    } else {
      currentDeliveredThrust.e += MAX(1024, thrustMapper.thrust_map.e - currentDeliveredThrust.e);
    }
    
    if (currentDeliveredThrust.f < thrustMapper.thrust_map.f) {
      currentDeliveredThrust.f += MIN(1024, thrustMapper.thrust_map.f - currentDeliveredThrust.f);
    } else {
      currentDeliveredThrust.f += MAX(1024, thrustMapper.thrust_map.f - currentDeliveredThrust.f);
    }
    
    if (currentDeliveredThrust.g < thrustMapper.thrust_map.g) {
      currentDeliveredThrust.g += MIN(1024, thrustMapper.thrust_map.g - currentDeliveredThrust.g);
    } else {
      currentDeliveredThrust.g += MAX(1024, thrustMapper.thrust_map.g - currentDeliveredThrust.g);
    }
    
    if (currentDeliveredThrust.h < thrustMapper.thrust_map.h) {
      currentDeliveredThrust.h += MIN(1024, thrustMapper.thrust_map.h - currentDeliveredThrust.h);
    } else {
      currentDeliveredThrust.h += MAX(1024, thrustMapper.thrust_map.h - currentDeliveredThrust.h);
    }

    //pseudo code
    //if Tset > T
        //T = T + min(1024,residual)
    //else
        //T = T - min(1024,residual)
    
    //send through CAN to motors
    sendToMotors();
}

int16_t* Overseer::getThrusters()
{
    thrusterValues[0] = thrustMapper.thrust_map.a;
    thrusterValues[1] = thrustMapper.thrust_map.b;
    thrusterValues[2] = thrustMapper.thrust_map.c;
    thrusterValues[3] = thrustMapper.thrust_map.d;
    thrusterValues[4] = thrustMapper.thrust_map.e;
    thrusterValues[5] = thrustMapper.thrust_map.f;
    thrusterValues[6] = thrustMapper.thrust_map.g;
    thrusterValues[7] = thrustMapper.thrust_map.h;
    
    return thrusterValues;
}


// FOR DEBUGGING USE ONLY!!!
vect8 Overseer::getThrust_Map(void)
{
  return thrustMapper.thrust_map;
}

ThrustMapper Overseer::getThrustMapper(void)
{
  return thrustMapper;
}

vect6 Overseer::getTargetForce(void)
{
  return target_force;
}

int Overseer::areOverseerAndMapperCommunicating(void)
{
  vect6 currentForceVector = thrustMapper.getCurrentForceVector();
  
  return (target_force.L.x == currentForceVector.L.x && target_force.L.y == currentForceVector.L.y && target_force.L.z == currentForceVector.L.z && target_force.R.x == currentForceVector.R.x && target_force.R.y == currentForceVector.R.y && target_force.R.z == currentForceVector.R.z);
}

