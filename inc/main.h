#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "init.h"

#define CAN_ID 0x13

#define Main_Camera_Servo_PWM_Pin GPIO_PIN_0
#define Main_Camera_Servo_PWM_GPIO_Port GPIOA
#define Alt_Camera_Servo_PWM_Pin GPIO_PIN_3
#define Alt_Camera_Servo_PWM_GPIO_Port GPIOA
#define Sonar_DAC_Pin GPIO_PIN_4
#define Sonar_DAC_GPIO_Port GPIOA
#define Sonar_Pwm_Pin GPIO_PIN_5
#define Sonar_Pwm_GPIO_Port GPIOA

#endif /* __MAIN_H */
