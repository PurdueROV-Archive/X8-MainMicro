//put all of your #includes into main.h file
#include "main.h"

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
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX  
    	* UART_HandleTypeDef huart1;		*/


/* 	USEFULL FUNCTIONS

	RED = ?; BLUE = ?; GREEN = ?; YELLOW = ?

	void LedOn(int ledNum);
	void LedOff(int ledNum);
	void LedToggle(int ledNum);
*/

int main(void) {

	//initializes all of the pins!
	initEverything();


	while (1) {

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

}

//this is run when a serial message is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){

}