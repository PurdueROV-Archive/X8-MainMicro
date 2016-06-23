#ifndef __INIT_H
#define __INIT_H

#include "main.h"

extern TIM_MasterConfigTypeDef sMasterConfig;
extern TIM_OC_InitTypeDef sConfigOC;

extern CAN_HandleTypeDef hcan2;

extern DAC_HandleTypeDef hdac;

extern ETH_HandleTypeDef heth;

extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart3;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;



void initEverything(void);

// Function declarations
void SystemClock_Config(void);
void Error_Handler(void);


void MX_GPIO_Init(void);
void MX_USART3_UART_Init(void);

// Initializes timer settings for pwm
void initPwm(void);

// Configurres the I2C with DMA
void initI2C(void);

// Configures the DMA nested vector interrupt priorities
void MX_DMA_Init(void);

// Configures the can bus
void initCan(void);

// Configures the shift register pins
void initShift(void);

// Function for initializing and using leds
void initDebugLeds(void);
void LedOn(int led);
void LedOff(int led);
void LedToggle(int led);

void HAL_MspInit(void);

#endif
