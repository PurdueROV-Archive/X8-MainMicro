#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "print.h"
#include <stdint.h>
#include <string.h>
#include "init.h"
#include <stm32f407xx.h>

#define POW_CAN_ID  0x11
#define APP_CAN_ID  0x12
#define CAN_ID      0x13

#define INSIDE_BLUE  11
#define OUTSIDE_BLUE 14
#define GREEN        12
#define RED          13

#define MAX(x,y) ((x > y) ? (x) : (y))
#define MIN(x,y) ((x > y) ? (y) : (x))

#define Main_Camera_Servo_PWM_Pin       GPIO_PIN_4
#define Main_Camera_Servo_PWM_GPIO_Port GPIOB
#define Alt_Camera_Servo_PWM_Pin        GPIO_PIN_3
#define Alt_Camera_Servo_PWM_GPIO_Port  GPIOA
#define Sonar_DAC_Pin                   GPIO_PIN_4
#define Sonar_DAC_GPIO_Port             GPIOA
#define Sonar_Pwm_Pin                   GPIO_PIN_5
#define Sonar_Pwm_GPIO_Port             GPIOA

#define MOTOR_OK 0xAB


#endif /* __MAIN_H */
