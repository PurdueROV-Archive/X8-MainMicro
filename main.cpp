
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

	RED; BLUE; GREEN; ORANGE

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

uint8_t buffer[2] = {'A', 'B'};
uint8_t serialInBuffer[SERIAL_BUFFER_SIZE] = {'z', 'y', 'x', 'w', 't', 'r', 's', '\0'};
uint8_t serialOutBuffer[SERIAL_BUFFER_SIZE] = {'h', 'e', 'l', 'l', 'l', 'o', 'M', 'a', 't', 't', 'C', 'M', 'o', 'l', 'o', '\0'};

PIController piController;
vect6 force_output;
int16_t * force_input;

PacketIn *packet;

int main(void) {
    //volatile uint_fast8_t RampTicker;
    
	//initializes all of the pins!
	initEverything();

	HAL_UART_Receive_DMA(&huart3, (uint8_t*)serialInBuffer, SERIAL_BUFFER_SIZE);


	packet = new PacketIn();
	HAL_UART_Receive_DMA(&huart3, packet->getArray(), SERIAL_BUFFER_SIZE);

	// IMU init
    IMU imu = IMU(&hi2c1);

    // PIController inits
	PIController piController = PIController();
	piController.start();


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

	//packet->recieve();
	while (1) {



		if(HAL_UART_Transmit_DMA(&huart3, packet->getArray(), SERIAL_BUFFER_SIZE) == HAL_OK)
		{
			//LedOn(ORANGE);
			//HAL_UART_Transmit_DMA(&huart3, packet->getArray(), SERIAL_BUFFER_SIZE);
			//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}

		imu.retrieve();
		piController.sensorInput(vect3Make((int) (imu.getX() * 10000), (int) (imu.getY() * 10000), (int) (imu.getZ() * 10000)), 
			vect3Make(0,0,0), HAL_GetTick());
		force_output.R = piController.getOutput();

		//LedToggle(BLUE);
        HAL_Delay(500);
		//first message
		//hcan2.pTxMsg->DLC = 1;
		//hcan2.pTxMsg->Data[i] = 0;
		//second and third

		//HAL_CAN_Transmit(&hcan2, 100); //second
		//HAL_CAN_Transmit(&hcan2, 100); //third

		/*LedOn(GREEN);
		HAL_Delay(100);
		LedOff(GREEN);
		HAL_Delay(100);
        if (RampTicker >= 20)
        {
            overseer.doRamping();
            RampTicker = 0;
        }*/
	}
}

/* This is where messages from CAN are received */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle){

	//example on how to use this in callback function
	if ((CanHandle->pRxMsg)->StdId == 0x11 && (CanHandle->pRxMsg)->IDE == CAN_ID_STD){

	}

	//restarts the interrupt
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}

//this is run when the a serial message is sent
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
	//LedToggle(BLUE);
}

//this is run when a serial message is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
	HAL_UART_Receive_DMA(&huart3, (uint8_t *)packet->getArray(), SERIAL_BUFFER_SIZE);

	packet->recieve();

	force_input = packet->getThrusters();
	force_output = vect6Make(force_input[0], force_input[1], force_input[2], force_input[3], force_input[4], force_input[5]);
	piController.setNewRotation(force_output.R);

	LedToggle(RED);
}
