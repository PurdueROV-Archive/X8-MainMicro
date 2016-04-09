//put all of your #includes into main.h file
#include "main.h"

#include "print.h"
#include "PacketIn.h"
#include "PacketOut.h"
#include "matrices.h"
#include "imu.h"
#include "pressure.h"
#include "pi_controller.h"


/*CAN2 GPIO Configuration    
    PB5  ------> CAN2_RX
    PB6  ------> CAN2_TX 
		* CAN_HandleTypeDef hcan2 		 	*/

/*I2C1 GPIO Configuration    
    PB7  ------> I2C1_SDA
    PB8  ------> I2C1_SCL 
		* I2C_HandleTypeDef hi2c1;  		*/

/*TIM5 GPIO Configuration    
    PA0  ------> TIM5_CH1
		* Main Camera servo
		* TIM_OC_InitTypeDef sConfigOC;
		* TIM_HandleTypeDef htim5
		* TIM_CHANNEL_1

    PA3  ------> TIM5_CH4
		* Alternate camera servo
		* TIM_OC_InitTypeDef sConfigOC;
		* TIM_HandleTypeDef htim5
		* TIM_CHANNEL_4						*/

/*USART1 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    	* UART_HandleTypeDef huart3;		*/


/* 	USEFULL FUNCTIONS

	RED; BLUE; GREEN; YELLOW

	void LedOn(int ledNum);
	void LedOff(int ledNum);
	void LedToggle(int ledNum);
*/

/*
	Example how to send can code

	CanHandle.pTxMsg->DLC = 3; //sets the size of the message in bytes. Max 8 bytes per message

	//sets the information that is sent over the message
	CanHandle.pTxMsg->Data[0] = 5;
    CanHandle.pTxMsg->Data[1] = 246;
    CanHandle.pTxMsg->Data[2] = 17;

	HAL_CAN_Transmit(&hcan2, 10);  //sends the message
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
	//initializes all of the pins!
	initEverything();

	//LedOn(BLUE);
	//LedOn(GREEN);
	//LedOn(RED);		
	//LedOn(ORANGE); 

	packet = new PacketIn();
	packetOut = new PacketOut();
	

	HAL_UART_Receive_DMA(&huart3, packet->getArray(), SERIAL_IN_BUFFER_SIZE);


	packetOut->setThrusterStatus(1);
	packetOut->setPressure(1);
	packetOut->setTemp(36);
	packetOut->setIMUA(1);	// Linear x
	packetOut->setIMUB(1);	// Linear y
	packetOut->setIMUC(1);	// Linear z
	packetOut->setIMUD(1);	// Rotational x
	packetOut->setIMUE(1);	// Rotational y
	packetOut->setIMUF(1);	// Rotational z
	packetOut->setIMUG(1);	// Gyro x
	packetOut->setIMUH(1);	// Gyro y
	packetOut->setIMUI(1);	// Gyro z


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

	//sets the packet size
	hcan2.pTxMsg->DLC = 8;


	while (1) {

		// Update piController's sensor data and compute its PID modulated output to the Rotational force vector.
		imu.get_linear_accel(); // Gets linear movement
		imu.retrieve_euler(); // Gets angular movement
		//piController.sensorInput(vect3Make((int16_t) (imu.rX() * 1000), (int16_t) (imu.rY() * 1000), (int16_t) (imu.rZ() * 1000)),
			//vect3Make(0,0,0/*(int16_t) (imu.aX() * 1000), (int16_t) (imu.aY() * 1000), (int16_t) (imu.aZ() * 1000)*/), HAL_GetTick());
		//force_output.R = piController.getOutput();

		// Pressure Sensor:
		pressure_mbar = pressure.getPressure(ADC_4096); // Returns mbar pressure from sensor.

		// Update PacketOut Data:
		packetOut->setThrusterStatus(1);
		packetOut->setPressure(pressure_mbar);
		packetOut->setTemp(36);
		packetOut->setIMUA(imu.lX());	// Linear x 	
		packetOut->setIMUB(imu.lY());	// Linear y 	
		packetOut->setIMUC(imu.lZ());	// Linear z 	
		packetOut->setIMUD(imu.rX());	// Rotational x 
		packetOut->setIMUE(imu.rY());	// Rotational y 
		packetOut->setIMUF(imu.rZ());	// Rotational z 
		packetOut->setIMUG(0);			// Gyro x?
		packetOut->setIMUH(0);			// Gyro y?
		packetOut->setIMUI(0);			// Gyro z?

		// DEBUG TEST:
		if (pressure_mbar >500 && pressure_mbar < 1500)
			LedToggle(BLUE);
		
		//long end = 0;
		/*LedToggle(BLUE);
		while (end < pressure_mbar)
		{
			HAL_Delay(1);
			end++;
		}
		LedToggle(BLUE);
		*/

		// CAN Transmission
		if (RECEIVED_NEW_DATA) {
			int16_t* thrusters =  packet->getThrusters();


			//sets the info for the logitudinal forces
			hcan2.pTxMsg->Data[0] =	'L';

			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[0], 6);
			hcan2.pTxMsg->Data[7] = packet->getArray()[15]; //Pump PWM Value

			
			if (HAL_CAN_Transmit(&hcan2, 100) == HAL_OK) {
				LedOn(BLUE);
				LedOff(RED);
			} else {
				LedOn(RED);
				LedOff(BLUE);
			}


			//sets the info for the rotational forces
			hcan2.pTxMsg->Data[0] =	'R';

			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[3], 6);
			hcan2.pTxMsg->Data[7] = packet->getArray()[18]; //The PID Control byte

			HAL_CAN_Transmit(&hcan2, 100); //send the rotational forces

			//Send serial data back
			HAL_UART_Transmit_DMA(&huart3, hcan2.pTxMsg->Data, 8);

			RECEIVED_NEW_DATA = false;
		}

		HAL_Delay(10);


		LedToggle(ORANGE);
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

	//restarts the interrupt
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}

//this is run when the a serial message is sent
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){

}

//this is run when a serial message is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){

	//set the Serial to read more data again
	HAL_UART_Receive_DMA(&huart3, (uint8_t *)packet->getArray(), SERIAL_IN_BUFFER_SIZE);


	packet->recieve();

	//packetOut->send();

	//force_input = packet->getThrusters();
	//force_output = vect6Make(force_input[0], force_input[1], force_input[2], force_input[3], force_input[4], force_input[5]);
	//piController.setNewRotation(force_output.R);


	//Indicate that we have new data, so send out can messages and other things
	RECEIVED_NEW_DATA = true;


	//LedToggle(RED);
}
