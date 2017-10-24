#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32.h"

void SPI1_init(uint16_t SPI_BaudRatePrescaler);
void SPI2_init();

void SPI_LCD_CS_HIGH();
void SPI_LCD_CS_LOW();
void SPI_TP_CS_HIGH();
void SPI_TP_CS_LOW();
void SPI_RST_HIGH();
void SPI_RST_LOW();
void GPIO_SPI_init();

uint16_t SPI_receive(SPI_TypeDef* SPIx);
