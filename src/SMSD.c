#include "smsd.h"


void GPIO_SMSD_init()
{
  GPIO_InitTypeDef  STEP_SMSD_init;  // создаем структуру

  STEP_SMSD_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  STEP_SMSD_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  STEP_SMSD_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  STEP_SMSD_init.GPIO_Speed = GPIO_Speed_40MHz;  // Скорость низкая
  STEP_SMSD_init.GPIO_Pin = GPIO_Pin_10; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &STEP_SMSD_init);  // Функция, выполняющая настройку портов

  GPIO_InitTypeDef  DIR_SMSD_init;  // создаем структуру

  DIR_SMSD_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  DIR_SMSD_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  DIR_SMSD_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  DIR_SMSD_init.GPIO_Speed = GPIO_Speed_40MHz;  // Скорость низкая
  DIR_SMSD_init.GPIO_Pin = GPIO_Pin_11; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &DIR_SMSD_init);  // Функция, выполняющая настройку портов
    
}     

void SMSD_STEP_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void SMSD_STEP_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

  
void SMSD_DIR_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void SMSD_DIR_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}
