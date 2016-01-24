#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>

#include "main.h"
#include "stm32f4xx_hal_uart.h"

/* UART handler declaration */
extern UART_HandleTypeDef UartHandle;

void initPrint(UART_HandleTypeDef handler);

void printInt(int num);
void printDouble(double num);
void printString(char* phrase);
