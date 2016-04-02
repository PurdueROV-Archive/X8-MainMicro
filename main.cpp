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
PacketIn *packet;
PacketOut *packetOut;

int canFlag = 0;

int main(void) {
	//initializes all of the pins!
	initEverything();



	packet = new PacketIn();
	packetOut = new PacketOut();


	HAL_UART_Receive_DMA(&huart3, packet->getArray(), SERIAL_IN_BUFFER_SIZE);


	packetOut->setThrusterStatus(1);
	packetOut->setPressure(1);
	packetOut->setTemp(36);
	packetOut->setIMUA(1);
	packetOut->setIMUB(1);
	packetOut->setIMUC(1);
	packetOut->setIMUD(1);
	packetOut->setIMUE(1);
	packetOut->setIMUF(1);
	packetOut->setIMUG(1);
	packetOut->setIMUH(1);
	packetOut->setIMUI(1);



	// IMU init
	IMU imu = IMU(&hi2c1);

	// pressure init
	Pressure pressure = Pressure(ADDRESS_HIGH);
	pressure.reset();
	pressure.begin();

    // PIController inits
	PIController piController = PIController();
	piController.start();
	piController.setNewRotation(vect3Make(0,0,0));
	piController.setNewP(0.001);
	piController.setNewI(0.001);


	while (1) {

		// Update piController's sensor data and compute its PID modulated output to the Rotational force vector.
		imu.get_linear_accel(); // Gets linear movement
		imu.retrieve_euler(); // Gets angular movement
		piController.sensorInput(vect3Make((int16_t) (imu.rX() * 1000), (int16_t) (imu.rY() * 1000), (int16_t) (imu.rZ() * 1000)), 
			vect3Make(0,0,0/*(int16_t) (imu.aX() * 1000), (int16_t) (imu.aY() * 1000), (int16_t) (imu.aZ() * 1000)*/), HAL_GetTick());
		force_output.R = piController.getOutput();

		// Pressure Sensor:
		// sensor.getPressure(ADC_4096); // Returns mbar pressure from sensor.

		// CAN Transmission
		canFlag = 1;
		if(canFlag == 1)
		{
			int16_t* thrusters =  packet->getThrusters();

			//int16_t array[3] = {255, 1000, 2000};
			//sets the packet size
			hcan2.pTxMsg->DLC = 8;

			//sets the info for the logitudinal forces
			hcan2.pTxMsg->Data[0] =	'L';

			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[0], 6);

			//hcan2.pTxMsg->Data[1] = (force_output.L.x  >> 8);
			//hcan2.pTxMsg->Data[2] = force_output.h.x;
			//hcan2.pTxMsg->Data[3] = (force_output.L.y  >> 8);
			//hcan2.pTxMsg->Data[4] = force_output.L.y;
			//hcan2.pTxMsg->Data[5] = (force_output.L.z  >> 8);
			//hcan2.pTxMsg->Data[6] = force_output.L.z;
			hcan2.pTxMsg->Data[7] = packet->getArray()[15]; //Pump ESC byte

			if (HAL_CAN_Transmit(&hcan2, 100) == HAL_OK) {
				LedToggle(BLUE);
			}

			HAL_UART_Transmit_DMA(&huart3, hcan2.pTxMsg->Data, 8);

			//sets the info for the rotational forces
			hcan2.pTxMsg->Data[0] =	'R';

			memcpy(&hcan2.pTxMsg->Data[1], &thrusters[3], 6);

			//hcan2.pTxMsg->Data[1] = (force_output.R.x  >> 8);
			//hcan2.pTxMsg->Data[2] = force_output.R.x;
			//hcan2.pTxMsg->Data[3] = (force_output.R.y  >> 8);
			//hcan2.pTxMsg->Data[4] = force_output.R.y;
			//hcan2.pTxMsg->Data[5] = (force_output.R.z  >> 8);
			//hcan2.pTxMsg->Data[6] = force_output.L.z;
			hcan2.pTxMsg->Data[7] = packet->getArray()[18]; //The PID Control byte

			HAL_CAN_Transmit(&hcan2, 100); //send the rotational forces

			//HAL_UART_Transmit_DMA(&huart3, hcan2.pTxMsg->Data, 8);

			canFlag = 0;
		}

		HAL_Delay(1);

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


	//sets the canFlag to one to indicate to the main loop that it should send can commands
	canFlag = 1;



	LedToggle(RED);
}
