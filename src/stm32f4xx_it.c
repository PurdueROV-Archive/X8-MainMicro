#include "main.h"



void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  }
}


void SVC_Handler(void)
{
  HAL_Delay(500);
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
}


void DebugMon_Handler(void)
{

}


void PendSV_Handler(void)
{
}


void SysTick_Handler(void)
{
  HAL_IncTick();
  //RampTicker++;
}

void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}

void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

void CAN2_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_RX1_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan2);
}



void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}


void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);
}
