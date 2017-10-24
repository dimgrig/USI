/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : stm32.h
 *    Description : board support include file
 *
 *    History :
 *    1. Date        : 07, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: #4 $
 **************************************************************************/

//#include "stm32l1xx_conf.h"
#include "stm32l1xx.h"
//#include "arm_comm.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_H
#define STM32_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum
{
  LED1 = 0,
  LED2 = 1
} Led_TypeDef;

typedef enum
{
  BUTTON_WAKEUP = 0
} Button_TypeDef;

typedef enum
{
	BUTTON_MODE_GPIO = 0,
	BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#define LEDn                            2

#define LED1_PIN                        GPIO_Pin_10
#define LED1_GPIO_PORT                  GPIOE
#define LED1_GPIO_CLK                   RCC_AHBPeriph_GPIOE

#define LED2_PIN                        GPIO_Pin_11
#define LED2_GPIO_PORT                  GPIOE
#define LED2_GPIO_CLK                   RCC_AHBPeriph_GPIOE

#define BUTTONn                         1

#define WAKEUP_BUTTON_PIN               GPIO_Pin_0
#define WAKEUP_BUTTON_GPIO_PORT         GPIOA
#define WAKEUP_BUTTON_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define WAKEUP_BUTTON_EXTI_LINE         EXTI_Line0
#define WAKEUP_BUTTON_EXTI_PORT_SOURCE  EXTI_PortSourceGPIOA
#define WAKEUP_BUTTON_EXTI_PIN_SOURCE   EXTI_PinSource0
#define WAKEUP_BUTTON_EXTI_IRQn         EXTI0_IRQn

void STM32_LEDInit(Led_TypeDef Led);
void STM32_LEDOn(Led_TypeDef Led);
void STM32_LEDOff(Led_TypeDef Led);
void STM32_LEDToggle(Led_TypeDef Led);
void STM32_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM32_PBGetState(Button_TypeDef Button);

#ifdef __cplusplus
}
#endif

#endif /* STM32_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
