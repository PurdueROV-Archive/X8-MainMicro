#include "main.h"

#include "print.h"
#include "packet_in.h"
#include "packet_out.h"
#include "matrices.h"
#include "imu.h"
#include "pressure.h"
#include "servo.h"
#include "pi_controller.h"


/*
 * CAN2 GPIO Configuration
 * PB5  ------> CAN2_RX
 * PB6  ------> CAN2_TX 
 * CAN_HandleTypeDef hcan2
 *
 */

/*
 * I2C1 GPIO Configuration
 * PB7  ------> I2C1_SDA
 * PB8  ------> I2C1_SCL 
 * I2C_HandleTypeDef hi2c1;
 *
 */


/*
 * TIM5 GPIO Configuration
 * PA0  ------> TIM5_CH1
 * Main Camera servo
 * TIM_OC_InitTypeDef sConfigOC;
 * TIM_HandleTypeDef htim5
 * TIM_CHANNEL_1
 *
 * PA3  ------> TIM5_CH4
 * Alternate camera servo
 * TIM_OC_InitTypeDef sConfigOC;
 * TIM_HandleTypeDef htim5
 * TIM_CHANNEL_4
 *
 */

/*
 * USART1 GPIO Configuration
 * PB10  ------> USART3_TX
 * PB11  ------> USART3_RX
 * UART_HandleTypeDef huart3;
 *
 */


/*
 * USEFULL FUNCTIONS
 *
 * RED; BLUE; GREEN; YELLOW
 * void LedOn(int ledNum);
 * void LedOff(int ledNum);
 * void LedToggle(int ledNum);
 *
 */


/*
 * Example how to send can code
 *
 * Sets the size of the packet. Max 8
 * CanHandle.pTxMsg->DLC = 8; 
 *
 * Set up the data
 * CanHandle.pTxMsg->Data[0] = 5;
 * CanHandle.pTxMsg->Data[1] = 246;
 * CanHandle.pTxMsg->Data[2] = 17;
 *
 * Send the message over can
 * HAL_CAN_Transmit(&hcan2, 10);
 *
 */


/* Variables used in the motor controlling code */
PIController piController; //stabalization controller structure
vect6 force_output;	//vector containing desired logitudinal rotational force for the ROV
int16_t * force_input;
float pressure_mbar;
PacketIn *packet;
PacketOut *packetOut;

bool RECEIVED_NEW_DATA = 0;

int main(void) {
	// Initializes all of the pins!
	initEverything();

	packet = new PacketIn();
	packetOut = new PacketOut();
	
	HAL_UART_Receive_DMA(&huart3, packet->getArray(), PACKET_IN_LENGTH);


	packetOut->setThrusterStatus(1);
	packetOut->setPressure(1.0);
	packetOut->setTemp(36.0);
	packetOut->setIMU_Lx(1.0);	// Linear x
	packetOut->setIMU_Ly(1.0);	// Linear y
	packetOut->setIMU_Lz(1.0);	// Linear z
	packetOut->setIMU_Rx(1.0);	// Rotational x
	packetOut->setIMU_Ry(1.0);	// Rotational y
	packetOut->setIMU_Rz(1.0);	// Rotational z


	// IMU init
	IMU imu = IMU(&hi2c1);

	// Pressure init	
	Pressure pressure = Pressure(ADDRESS_HIGH, &hi2c1);
	pressure.reset();
	pressure.begin();

	// PIController inits
	PIController piController = PIController();
	piController.start();
	piController.setNewRotation(vect3Make(0,0,0));
	piController.setNewP(0.001);
	piController.setNewI(0.001);

	// Can Packet Size Init
	hcan2.pTxMsg->DLC = 8;

	// Camera Servo Init
	servo cameraServo = servo(&htim3, &sConfigOC, TIM_CHANNEL_1);
	cameraServo.setStart(0.725);
	cameraServo.setRange(1.39);

	while (1) {

		if (RECEIVED_NEW_DATA) {

			// IMU Sensor:
			/* Commented out until I2C isn't locking up
			imu.get_linear_accel(); // Gets linear movement
			imu.retrieve_euler(); // Gets angular movement
			*/

			// Pressure Sensor:
			/* Commented out until I2C isn't locking up
			pressure_mbar = pressure.getPressure(ADC_4096); // Returns mbar pressure from sensor.

			// Pressure Debug Test:
			if (pressure_mbar > 500 && pressure_mbar < 1500) {
				LedToggle(RED);
			}
			*/
			
			
			// PID Controller:
			/* Commented out until IMU working
			// Update piController's sensor data and compute its PID modulated output to the Rotational force vector.
			piController.sensorInput(vect3Make((int16_t) (imu.rX() * 1000), (int16_t) (imu.rY() * 1000), (int16_t) (imu.rZ() * 1000)),
			vect3Make((int16_t) (imu.aX() * 1000), (int16_t) (imu.aY() * 1000), (int16_t) (imu.aZ() * 1000)), HAL_GetTick());
			force_output.R = piController.getOutput();
			*/


			// Update PacketOut Data:
			packetOut->setThrusterStatus(1);
			packetOut->setTemp(36);

			/*
			packetOut->setPressure(pressure_mbar);
			packetOut->setIMU_Lx(imu.lX());	// Linear x 	
			packetOut->setIMU_Ly(imu.lY());	// Linear y 	
			packetOut->setIMU_Lz(imu.lZ());	// Linear z 	
			packetOut->setIMU_Rx(imu.rX());	// Rotational x 
			packetOut->setIMU_Ry(imu.rY());	// Rotational y 
			packetOut->setIMU_Rz(imu.rZ());	// Rotational z 
			*/


			/*
			// Commented out until full server testing is working
			cameraServo.set((packet->getCameraServo() <= 128 ? 90 - (packet->getCameraServo() * (180 / 256)) : (packet->getCameraServo() * (180 / 256))));
			*/



			int16_t* thrusters = packet->getThrusters();

			// Sets the info for the logitudinal forces
			hcan2.pTxMsg->Data[0] =	'L';
			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[0], 6);
			hcan2.pTxMsg->Data[7] = packet->getHydraulicsPump();
			
			// Send the longitudinal forces
			if (HAL_CAN_Transmit(&hcan2, 100) == HAL_OK) {
				LedOn(BLUE);
			} else {
				LedOff(BLUE);
			}

			// Sets the info for the rotational forces
			hcan2.pTxMsg->Data[0] =	'R';
			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[3], 6);
			hcan2.pTxMsg->Data[7] = packet->getPIDControl(); //The PID Control byte

			// Send the rotational forces
			if (HAL_CAN_Transmit(&hcan2, 100) == HAL_OK) {
				LedOn(BLUE);
			} else {
				LedOff(BLUE);
			}


		    // Send packet data back up
		    packetOut->send();

			RECEIVED_NEW_DATA = false;
		}

		LedToggle(ORANGE);


		//Delay Loop 10ms
		HAL_Delay(10);
	}
}


/* This is where messages from CAN are received */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle){

	//example on how to use this in callback function
	if ((CanHandle->pRxMsg)->StdId == POW_CAN_ID && (CanHandle->pRxMsg)->IDE == CAN_ID_STD){

		if ((CanHandle->pRxMsg)->DLC == 8) {
			
		}

		bool motor_status = 0xFF;

		for (int i = 0; i < (CanHandle->pRxMsg)->DLC; i++) {
			motor_status = motor_status && ((CanHandle->pRxMsg)->Data[i] == MOTOR_OK);
		}

	}

	// Restarts the interrupt
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}

//this is run when the a serial message is sent
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){

}

//this is run when a serial message is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){

	//set the Serial to read more data again
	HAL_UART_Receive_DMA(&huart3, (uint8_t *)packet->getArray(), PACKET_IN_LENGTH);


	packet->recieve();

	//force_input = packet->getThrusters();
	//force_output = vect6Make(force_input[0], force_input[1], force_input[2], force_input[3], force_input[4], force_input[5]);
	//piController.setNewRotation(force_output.R);


	//Indicate that we have new data, so send out can messages and other things
	RECEIVED_NEW_DATA = true;


	LedToggle(GREEN);
}
