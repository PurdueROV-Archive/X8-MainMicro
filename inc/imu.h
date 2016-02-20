#ifndef X8_IMU
#define X8_IMU

/* puts all of the #includes and # defines that you need here */
#include "main.h"

class IMU {


public:
	/* Put function declarations that the user will
	   want to use from the class here */


/*constructor */
    IMU(I2C_HandleTypeDef* handler);

    void retrieve(void);

    double getX(void);

    double getY(void);

    double getZ(void);



private:

    I2C_HandleTypeDef* I2C_handler;
    double xAngle;
    double yAngle;
    double zAngle;

};

#endif
