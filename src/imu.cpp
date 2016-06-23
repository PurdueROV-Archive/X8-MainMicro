#include "imu.h"

/*
 * This library uses I2C to communicate with the BNO055 Inertial Measurement Unit.
 * Upon request, the IMU will send Euler angles x y and z in degrees.
 * To set up communication with the IMU, use the constructor IMU( I2C handler pointer ).
 * 		where 'I2C handler pointer' is a pointer of type I2C_HandleTypeDef*
 *
 *
 * To read angles from the IMU, run the retrieve() function
 * which will receive and parse the angles and store them in the private variables
 * xAngle, yAngle, and zAngle
 * These variables are available to the user using the functions getX(), getY(), or getZ().
 */

IMU::IMU(I2C_HandleTypeDef* handler) {

	// This variable is what you plug into the send function
	I2C_handler = handler;

	page_flag = 0xff;
	select_page(0);

	check_id();

	select_page(0);
	dt[0] = IMU_UNIT_SEL;
	dt[1] = 0x00;
	la[0] = 0.0;
	la[1] = 0.0;
	la[2] = 0.0;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
    this->i2c_wait();

	uint8_t chip_mode = 0x08;
	change_fusion_mode(chip_mode);
}

// Retrieves the data from the sensor and stores it into variables
bool IMU::retrieve_euler(void) {

	/* this function retrieves the data from the sensor and then stores it into
     * xAngle, yAngle, and zAngle so that you can call getX, getY, and getZ later whenever you need
     * the angle data. This means that you must call retrieve before you call the other functions
     * if you want to get new angle data
     */

	select_page(0);

	dt[0] = IMU_EULER_H_LSB;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
    this->i2c_wait();

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 6);
    this->i2c_wait();

	zAngle = ((char) dt[1] << 8 | (char) dt[0]);
	xAngle = ((char) dt[3] << 8 | (char) dt[2]);
	yAngle = ((char) dt[5] << 8 | (char) dt[4]);

	if (yAngle > 0)
		yAngle -= 2880;
	else
		yAngle += 2880;

	return true;
}

void IMU::get_linear_accel(void) {
	select_page(0);

	dt[0] = IMU_LINEAR_ACC_X_LSB;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
    this->i2c_wait();

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 6);
    this->i2c_wait();

	la[0] = ((char) dt[1] << 8 | (char) dt[0]);
	la[1] = ((char) dt[3] << 8 | (char) dt[2]);
	la[2] = ((char) dt[5] << 8 | (char) dt[4]);
}

// Returns the angle with respect to the X axis
int16_t IMU::rX(void) {
	return xAngle;
}

// Returns the angle with respect to the Y axis
int16_t IMU::rY(void) {
	return yAngle;
}

// Returns the angle with respect to the Z axis
int16_t IMU::rZ(void) {
	return zAngle;
}

// Returns the linear acceleration with respect to the X axis.
int16_t IMU::lX(void) {
	return la[0];
}

// Returns the linear acceleration with respect to the Y axis.
int16_t IMU::lY(void) {
	return la[1];
}

// Returns the linear acceleration with respect to the Z axis.
int16_t IMU::lZ(void) {
	return la[2];
}

// Returns the rotation as a vect3
vect3 IMU::get_rot(void) {
	vect3 output;

	output.x = xAngle;
	output.y = yAngle;
	output.z = zAngle;

	return output;
}

uint8_t IMU::select_page(uint8_t page) {
	if (page != page_flag){
		dt[0] = IMU_PAGE_ID;
		if (page == 1) {
			dt[1] = 1;  // Select page 1
		} else {
			dt[1] = 0;  // Select page 0
		}

		HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
        this->i2c_wait();

		dt[0] = IMU_PAGE_ID;

		HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
        this->i2c_wait();

		HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
        this->i2c_wait();

		page_flag = dt[0];
	}
	return page_flag;
}

void IMU::check_id(void) {

	select_page(0);
	dt[0] = IMU_CHIP_ID;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
    this->i2c_wait();

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 7);
    this->i2c_wait();

	if (dt[0] == IMU_CHIP_ID) {
		ready_flag = 1;
	} else {
		ready_flag = 0;
	}

	if (dt[1] == IMU_ACC_ID) {
		ready_flag |= 2;
	}

	if (dt[2] == IMU_MAG_ID) {
		ready_flag |= 4;
	}

	if (dt[2] == IMU_MAG_ID) {
		ready_flag |= 8;
	}

	bootldr_rev_id = dt[5] << 8 | dt[4];
	sw_rev_id = dt[6];
}

void IMU::change_fusion_mode(uint8_t mode) {
	// Changes fusion mode to configure or send angles via I2C
	uint8_t current_mode;

	select_page(0);
	current_mode = check_operating_mode();
	switch (mode) {
		case CONFIGMODE:
			dt[0] = IMU_OPR_MODE;
			dt[1] = mode;

			HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
            this->i2c_wait();

			HAL_Delay(19);    // wait 19mS
			break;
		case MODE_IMU:
		case MODE_COMPASS:
		case MODE_M4G:
		case MODE_NDOF_FMC_OFF:
        case MODE_NDOF:
            if (current_mode != CONFIGMODE) { // Can we change the mode directry?
                dt[0] = IMU_OPR_MODE;
                dt[1] = CONFIGMODE;
                HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
                this->i2c_wait();

                HAL_Delay(19); // Wait 19mS
            }
            dt[0] = IMU_OPR_MODE;
            dt[1] = mode;
            HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
            this->i2c_wait();

            HAL_Delay(7);
            break;
        default:
			break;
	}
}

uint8_t IMU::check_operating_mode(void) {
	// Initialize operating mode
	select_page(0);
	dt[0] = IMU_OPR_MODE;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
    this->i2c_wait();

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
    this->i2c_wait();

	return dt[0];
}

void IMU::i2c_wait(int8_t timeout /* = 5 */) {
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY
           && timeout-- > 0) {
        HAL_Delay(1);
    }
}



