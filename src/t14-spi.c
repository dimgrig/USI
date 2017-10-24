#include "t14-spi.h"


         
void SPI1_init(uint16_t SPI_BaudRatePrescaler)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //////////////!!!!!!!!
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;///////////////!!!!!!!!!!!!! SPI_BaudRatePrescaler_4
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI2_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	SPI_I2S_DeInit(SPI2);
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; ///////////////!!!!!!!!!!!!!
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = 32;///////////////!!!!!!!!!!!!!
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

uint16_t SPI_receive(SPI_TypeDef* SPIx)
{
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint16_t)SPI_I2S_ReceiveData(SPIx);
}

          
void GPIO_SPI_init()
{
  GPIO_InitTypeDef  CS_LCD_init;  // создаем структуру

  CS_LCD_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  CS_LCD_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  CS_LCD_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  CS_LCD_init.GPIO_Speed = GPIO_Speed_40MHz;  // Скорость низкая
  CS_LCD_init.GPIO_Pin = GPIO_Pin_8; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &CS_LCD_init);  // Функция, выполняющая настройку портов
  
  GPIO_InitTypeDef  CS_TP_init;  // создаем структуру

  CS_TP_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  CS_TP_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  CS_TP_init.GPIO_PuPd = GPIO_PuPd_UP;  // Без подтяжки
  CS_TP_init.GPIO_Speed = GPIO_Speed_40MHz;  // Скорость низкая
  CS_TP_init.GPIO_Pin = GPIO_Pin_9; // Светодиод на 15м выводе

  GPIO_Init(GPIOB, &CS_TP_init);  // Функция, выполняющая настройку портов
  
  GPIO_InitTypeDef  RST_init;  // создаем структуру

  RST_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  RST_init.GPIO_OType = GPIO_OType_PP;  // Двухтактный выход
  RST_init.GPIO_PuPd = GPIO_PuPd_NOPULL;  // Без подтяжки
  RST_init.GPIO_Speed = GPIO_Speed_40MHz;  // Скорость низкая
  RST_init.GPIO_Pin = GPIO_Pin_3; // Светодиод на 15м выводе

  GPIO_Init(GPIOA, &RST_init);  // Функция, выполняющая настройку портов
 
  
  GPIO_InitTypeDef TP_IRQ_init;
  EXTI_InitTypeDef exti;
  NVIC_InitTypeDef nvic;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 
  TP_IRQ_init.GPIO_Mode = GPIO_Mode_IN;
  TP_IRQ_init.GPIO_Pin = GPIO_Pin_0;
  TP_IRQ_init.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOB, &TP_IRQ_init);
  
 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
  
  exti.EXTI_Line = EXTI_Line0;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Falling;
  exti.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti);
 
  nvic.NVIC_IRQChannel = EXTI0_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
  
}     

void SPI_LCD_CS_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void SPI_LCD_CS_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

void SPI_TP_CS_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void SPI_TP_CS_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void SPI_RST_HIGH()
{
  GPIO_SetBits(GPIOA, GPIO_Pin_3);
}

void SPI_RST_LOW()
{
  GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}

/* рабочий код SPI*/
/*  
    SPI_I2S_SendData(SPI1, 15);
    uint16_t data1 = SPI_receive(SPI2);
 
    while(!s_Tick);
    s_Tick = 0;  
    if (data1 == 15)
       LEDsSet(0x3);

*/
