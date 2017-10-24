#include "t14-timer.h"



void TIMERS_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // LEDs Init
  STM32_LEDInit(LED1);
  STM32_LEDInit(LED2);

  // Timer2 Init
  // Enable Timer1 clock and release reset
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2,DISABLE);

  // Set timer period 0.1 sec
  TIM_TimeBaseInitStruct.TIM_Prescaler = 320;  // 10us resolution
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 50000;  // 500 ms
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
  // Enable update interrupt
  TIM_ITConfig(TIM2,TIM_FLAG_Update,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable timer counting
  TIM_Cmd(TIM2,ENABLE);
}


