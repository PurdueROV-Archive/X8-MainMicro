/*This folder will be used to hold all code used to initialize pins */

#include "init.h"
#include "stm32f4xx_hal.h"

TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfigOC;

CAN_HandleTypeDef hcan2;
static CanTxMsgTypeDef        TxMessage;
static CanRxMsgTypeDef        RxMessage;

DAC_HandleTypeDef hdac;

ETH_HandleTypeDef heth;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;


UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;


TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

//this function will call all of the other initialization functions
void initEverything(void)
{
	/* GPIO Ports Clock Enable */
 	__GPIOA_CLK_ENABLE();
 	__GPIOB_CLK_ENABLE();
 	__GPIOC_CLK_ENABLE();
 	__GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

 	//must be included to initially configure the library
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	initDebugLeds();

	HAL_MspInit();

	MX_DMA_Init();

	MX_GPIO_Init();
	MX_USART1_UART_Init();



	initI2C();
	initCan();
	initPwm();
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

}

void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
			__GPIOA_CLK_ENABLE();

}



void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(huart->Instance==USART1)
	{
				__USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* Peripheral DMA init*/

		hdma_usart1_rx.Instance = DMA2_Stream2;
		hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_NORMAL;
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&hdma_usart1_rx);

		__HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);

		hdma_usart1_tx.Instance = DMA2_Stream7;
		hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_NORMAL;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&hdma_usart1_tx);

		__HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}

}

/* checked! */
void initPwm(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    /**TIM2 GPIO Configuration    
    PA5     ------> TIM2_CH1 
    */
    /*GPIO_InitStruct.Pin = Sonar_Pwm_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(Sonar_Pwm_GPIO_Port, &GPIO_InitStruct);*/

    __TIM5_CLK_ENABLE();

    /**TIM5 GPIO Configuration    
    PA0     ------> TIM5_CH1
    PA3     ------> TIM5_CH4 
    */
    GPIO_InitStruct.Pin = Main_Camera_Servo_PWM_Pin|Alt_Camera_Servo_PWM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    htim5.Instance = TIM5;
  	htim5.Init.Prescaler = SystemCoreClock;
  	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  	htim5.Init.Period = 1000;
  	htim5.Init.ClockDivision = 0;
  	HAL_TIM_PWM_Init(&htim5);

  	/*sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  	HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);*/

  	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  	sConfigOC.Pulse = 500;
  	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;




	HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1);

  	HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4);


	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

}

/* checked! */
void initI2C(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/**I2C1 GPIO Configuration    
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL 
    */

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);




    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_i2c1_rx);

    __HAL_LINKDMA(&hi2c1,hdmarx,hdma_i2c1_rx);

    hdma_i2c1_tx.Instance = DMA1_Stream6;
    hdma_i2c1_tx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_i2c1_tx);

    __HAL_LINKDMA(&hi2c1,hdmatx,hdma_i2c1_tx);

    hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
 	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  	hi2c1.Init.OwnAddress1 = 0;
  	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  	hi2c1.Init.OwnAddress2 = 0;
  	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  	HAL_I2C_Init(&hi2c1);
}

void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

  	//USART DMA initialization
			__DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);


}

void initCan(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__CAN2_CLK_ENABLE();
    __CAN1_CLK_ENABLE();

    /**CAN2 GPIO Configuration    
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX 
    */

    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // Peripheral interrupt it
     HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
      HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);

      hcan2.Instance = CAN2;


		hcan2.pTxMsg = &TxMessage;
		hcan2.pRxMsg = &RxMessage;

        hcan2.Init.Prescaler = 2;
        hcan2.Init.Mode = CAN_MODE_NORMAL;
        hcan2.Init.SJW = CAN_SJW_1TQ;
        hcan2.Init.BS1 = CAN_BS1_6TQ;
        hcan2.Init.BS2 = CAN_BS2_8TQ;
        hcan2.Init.TTCM = DISABLE;
        hcan2.Init.ABOM = DISABLE;
        hcan2.Init.AWUM = DISABLE;
        hcan2.Init.NART = DISABLE;
        hcan2.Init.RFLM = DISABLE;
        hcan2.Init.TXFP = DISABLE;



		if(HAL_CAN_Init(&hcan2) != HAL_OK)
		{

		}

        //configures the fileter for the can communication
        CAN_FilterConfTypeDef  sFilterConfig;


		sFilterConfig.FilterNumber = 0;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
      	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterIdHigh = 0x0000;
		sFilterConfig.FilterIdLow = 0x0000;
		sFilterConfig.FilterMaskIdHigh = 0x0000;
		sFilterConfig.FilterMaskIdLow = 0x0000;
      	sFilterConfig.FilterFIFOAssignment = 0;
      	sFilterConfig.FilterActivation = ENABLE;
      	sFilterConfig.BankNumber = 0;

      	if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
		{

		}

      //sets up the communication information
      hcan2.pTxMsg->StdId = CAN_ID;  //the id of this microboard

      hcan2.pTxMsg->RTR = CAN_RTR_DATA;
      hcan2.pTxMsg->IDE = CAN_ID_STD;

		hcan2.pTxMsg->DLC = 1;
		hcan2.pTxMsg->Data[0] = 1;



        if(HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
		{

		}

}

void HAL_MspInit(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

//initialize the debugging leds
void initDebugLeds(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//enable the led clock
	 __HAL_RCC_GPIOD_CLK_ENABLE();

	//configures the led pin  
	GPIO_InitStruct.Pin = GPIO_PIN_11; 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);  

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
}

//turns on and turns off the led
void LedOn(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
}

//turns on and turns off the led
void LedOff(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

//turns on and turns off the led
void LedToggle(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	}
}

//configures the system clcok
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;

			__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//sets up the pins for can to be alternate functions
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(hcan->Instance==CAN2)
	{


		/* Peripheral clock enable */
		__CAN2_CLK_ENABLE();
		__CAN1_CLK_ENABLE();



		/**CAN1 GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
		HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
	}
}

//function used to handle errors
void Error_Handler(void)
{
  while(1)
  {
  }
}
