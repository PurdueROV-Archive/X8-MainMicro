/* Put all of your #includes and #defines up here   */

#include "imu.h"


IMU::IMU(I2C_HandleTypeDef* handler) {

    /*this variable is what you plug into the send function */
    I2C_handler = handler;
}

// retrieves the data from the sensor and stores it into variables
void IMU::retrieve(void) {

    /* In this function, I want you to retreive the data from the sensor and then store it into
     * xAngle, yAngle, and zAngle so that you can call getX, getY, and getZ later whenever you need
     * the angle data. This means that you must call retreive before you call the other functions
     * if you want to get new angle data*/

}

//returns the angle with respect to the X axis
double IMU::getX(void){
    return xAngle;
}

//returns the angle with respect to the Y axis
double IMU::getY(void){
    return yAngle;
}

//returns the angle with respect to the Z axis
double IMU::getZ(void){
    return zAngle;
}


