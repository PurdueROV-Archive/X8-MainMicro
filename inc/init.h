/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

// INCLUDES:

#include "stm32f4xx_hal.h"
#include "print.h"

// DEFINES:

#define POW_CAN_ID	0x11
#define APP_CAN_ID 	0x12
#define CAN_ID 0x13

#define GREEN 0
#define ORANGE 1
#define RED 2
#define BLUE 3

#define Main_Camera_Servo_PWM_Pin GPIO_PIN_0
#define Main_Camera_Servo_PWM_GPIO_Port GPIOA
#define Alt_Camera_Servo_PWM_Pin GPIO_PIN_3
#define Alt_Camera_Servo_PWM_GPIO_Port GPIOA
#define Sonar_DAC_Pin GPIO_PIN_4
#define Sonar_DAC_GPIO_Port GPIOA
#define Sonar_Pwm_Pin GPIO_PIN_5
#define Sonar_Pwm_GPIO_Port GPIOA

// FORWARD DECLARATIONS:

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
extern TIM_HandleTypeDef htim5;


// PROTOTYPE FUNCTIONS:

void initEverything(void);

/* function declarations */
void SystemClock_Config(void);
void Error_Handler(void);


void MX_GPIO_Init(void);
void MX_USART3_UART_Init(void);

/* initializes timer settings for pwm */
void initPwm(void);

/* configurres the I2C with DMA */
void initI2C(void);

/* configures the DMA nested vector interrupt priorities */
void MX_DMA_Init(void);

/* configures the can bus */
void initCan(void);

/*function for initializing and using leds */
void initDebugLeds(void);
void LedOn(int ledNum);
void LedOff(int ledNum);
void LedToggle(int ledNum);



void HAL_MspInit(void);

#endif