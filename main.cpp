//put all of your #includes into main.h file
#include "main.h"

#include "print.h"
#include "PacketIn.h"
#include "PacketOut.h"


#include "matrices.h"
#include "imu.h"
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
int main(void) {
	//initializes all of the pins!
	initEverything();



	packet = new PacketIn();
	packetOut = new PacketOut();
	HAL_UART_Receive_DMA(&huart3, packet->getArray(), SERIAL_IN_BUFFER_SIZE);
	//packetOut->setData();


	/*
	//sets the size of the message in bytes. Max 8 bytes per message
	hcan2.pTxMsg->DLC = 8;
	//sets the information that is sent over the message
	hcan2.pTxMsg->Data[0] = 0;
	hcan2.pTxMsg->Data[1] = 1;
	hcan2.pTxMsg->Data[2] = 0;
	hcan2.pTxMsg->Data[3] = 1;
	hcan2.pTxMsg->Data[4] = 0;
	hcan2.pTxMsg->Data[5] = 1;
	hcan2.pTxMsg->Data[6] = 0;
	hcan2.pTxMsg->Data[7] = 1;
	 */

	// IMU init
    IMU imu = IMU(&hi2c1);


    // PIController inits
	PIController piController = PIController();
	piController.start();

	//volatile uint_fast8_t RampTicker;

	//testing variables for motors
	uint16_t throttle = 7000;
	uint8_t motorAddress = 0x29;


	while (1) {

		//LedToggle(GREEN);
		//send back up the serial data for debugging
		if(HAL_UART_Transmit_DMA(&huart3, packet->getArray(), SERIAL_IN_BUFFER_SIZE) == HAL_OK)
		{

		}


		//LedToggle(BLUE);
        HAL_Delay(500);
		//initialize and send header message
		hcan2.pTxMsg->DLC = 1;
		hcan2.pTxMsg->Data[0] = 0;
		HAL_CAN_Transmit(&hcan2, 100); //header

		//initialize and send data message
		//sets the size of the message in bytes. Max 8 bytes per message
		hcan2.pTxMsg->DLC = 8;
		//sets the information that is sent over the message

		hcan2.pTxMsg->Data[0] = 0;
		hcan2.pTxMsg->Data[1] = 1;

		hcan2.pTxMsg->Data[2] = 0;
		hcan2.pTxMsg->Data[3] = 1;

		hcan2.pTxMsg->Data[4] = 0;
		hcan2.pTxMsg->Data[5] = 1;

		hcan2.pTxMsg->Data[6] = 0;
		hcan2.pTxMsg->Data[7] = 1;

		HAL_CAN_Transmit(&hcan2, 100); //thrusters 1-4

		//initialize and send data message
		//sets the size of the message in bytes. Max 8 bytes per message
		hcan2.pTxMsg->DLC = 8;
		//sets the information that is sent over the message

		hcan2.pTxMsg->Data[0] = 0;
		hcan2.pTxMsg->Data[1] = 1;

		hcan2.pTxMsg->Data[2] = 0;
		hcan2.pTxMsg->Data[3] = 1;

		hcan2.pTxMsg->Data[4] = 0;
		hcan2.pTxMsg->Data[5] = 1;

		hcan2.pTxMsg->Data[6] = 0;
		hcan2.pTxMsg->Data[7] = 1;

		HAL_CAN_Transmit(&hcan2, 100); //thrusters 5-8

		HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);

		/*imu.retrieve();  //receives data from the imu

		piController.sensorInput(vect3Make((int) (imu.getX() * 10000), (int) (imu.getY() * 10000), (int) (imu.getZ() * 10000)),
		vect3Make(0,0,0), HAL_GetTick());
		force_output.R = piController.getOutput();*/


        HAL_Delay(300);


		/*uint8_t temp[3] = {0x00, (throttle>>8), throttle};
		HAL_I2C_Master_Transmit(&hi2c1, motorAddress << 1, temp, 3, 100);
		while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
		{
			HAL_Delay(1);
		}*/


        /*if (RampTicker >= 20)
        {
            overseer.doRamping();
            RampTicker = 0;
        }*/
	}
}

/* This is where messages from CAN are received */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle){

	//example on how to use this in callback function
	if ((CanHandle->pRxMsg)->StdId == POW_CAN_ID && (CanHandle->pRxMsg)->IDE == CAN_ID_STD){

		if ((CanHandle->pRxMsg)->DLC == 8) {
			LedToggle(ORANGE);
		}

		bool motor_status = true;
		for (int i = 0; i < (CanHandle->pRxMsg)->DLC; i++) {
			motor_status = motor_status && ((CanHandle->pRxMsg)->Data[i] == MOTOR_OK);
		}

		if (motor_status) {
			LedOn(RED);
		} else {
			LedOff(RED);
		}

	}
	uint8_t buffer[3] = {'1','2','3'};
	HAL_UART_Transmit_DMA(&huart3, buffer, 3);

	//restarts the interrupt
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}

//this is run when the a serial message is sent
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){

}

//this is run when a serial message is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){

	HAL_UART_Receive_DMA(&huart3, (uint8_t *)packet->getArray(), SERIAL_IN_BUFFER_SIZE);

	packet->recieve();

	force_input = packet->getThrusters();
	force_output = vect6Make(force_input[0], force_input[1], force_input[2], force_input[3], force_input[4], force_input[5]);
	piController.setNewRotation(force_output.R);

	uint8_t i = 0;
	LedOn(GREEN);
	for (i = 0; i < 6; i++) {
		if (force_input[i]!= 1) LedOff(GREEN);
	}

	//sets the packet size
	hcan2.pTxMsg->DLC = 8;

	//sets the info for the logitudinal forces
	hcan2.pTxMsg->Data[0] =	'L';
	hcan2.pTxMsg->Data[1] = (force_output.L.x  >> 8);
	hcan2.pTxMsg->Data[2] = force_output.L.x;
	hcan2.pTxMsg->Data[3] = (force_output.L.y  >> 8);
	hcan2.pTxMsg->Data[4] = force_output.L.y;
	hcan2.pTxMsg->Data[5] = (force_output.L.z  >> 8);
	hcan2.pTxMsg->Data[6] = force_output.L.z;
	hcan2.pTxMsg->Data[7] = packet->getArray()[15]; //Pump ESC byte

	HAL_CAN_Transmit(&hcan2, 100); //send the longitudinal forces

	//sets the info for the rotational forces
	hcan2.pTxMsg->Data[0] =	'R';
	hcan2.pTxMsg->Data[1] = (force_output.R.x  >> 8);
	hcan2.pTxMsg->Data[2] = force_output.R.x;
	hcan2.pTxMsg->Data[3] = (force_output.R.y  >> 8);
	hcan2.pTxMsg->Data[4] = force_output.R.y;
	hcan2.pTxMsg->Data[5] = (force_output.R.z  >> 8);
	hcan2.pTxMsg->Data[6] = force_output.L.z;
	hcan2.pTxMsg->Data[7] = packet->getArray()[18]; //The PID Control byte

	HAL_CAN_Transmit(&hcan2, 100); //send the rotational forces
	//LedToggle(RED);
}
