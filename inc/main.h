#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "print.h"
#include <stdint.h>
#include <stm32f407xx.h>
#include "init.h"

//extern volatile uint_fast8_t RampTicker;
#define POW_CAN_ID	0x11
#define APP_CAN_ID 	0x12
#define CAN_ID 0x13

#define GREEN 0
#define ORANGE 1
#define RED 2
#define BLUE 3

#define MAX(x,y) (x > y) ? x : y
#define MIN(x,y) (x > y) ? y : x

#define Main_Camera_Servo_PWM_Pin GPIO_PIN_0
#define Main_Camera_Servo_PWM_GPIO_Port GPIOA
#define Alt_Camera_Servo_PWM_Pin GPIO_PIN_3
#define Alt_Camera_Servo_PWM_GPIO_Port GPIOA
#define Sonar_DAC_Pin GPIO_PIN_4
#define Sonar_DAC_GPIO_Port GPIOA
#define Sonar_Pwm_Pin GPIO_PIN_5
#define Sonar_Pwm_GPIO_Port GPIOA

<<<<<<< HEAD
#define MOTOR_OK 0xAB
=======
>>>>>>> origin/master

#endif /* __MAIN_H */
