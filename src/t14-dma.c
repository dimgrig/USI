/*
 * t14-dma.c
 *
 *  Created on: 28 окт. 2017 г.
 *      Author: Dima
 */

#include "t14-dma.h"
#include "stm32l1xx_dma.h"



void DMA_init(void)
{
	//DMA_InitTypeDef DMA_InitStructure;

	//RCC_APB1PeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// АЦП
//	DMA_DeInit(DMA1_Stream0);
//	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &adc_buffer[0];
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_BufferSize = ARRAY_LENGHT(adc_buffer);
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_Init(DMA2_Stream0, &DMA_InitStructure);


//	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
//	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DMA_BUFFER[0];
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
//
//	DMA_Cmd(DMA2_Stream0, ENABLE);
//



	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; //Variable used to setup the DMA

	ADC_InitTypeDef ADC_InitStructure;

	//--Enable DMA1 clock--
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//==Configure DMA1 - Channel1==
	DMA_DeInit(DMA1_Channel1); //Set DMA registers to default values
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //Address of peripheral the DMA must map to
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & DMA_BUFFER[0]; //Variable to which ADC values will be stored
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE; //Buffer size (8 because we using 8 channels)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;


	//Настройка Прерывание -по окончании трансфера

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //включаем прерывание по половине буфера
	DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE); //включаем прерывание по окончанию буфера
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //Initialise the DMA

	DMA_Cmd(DMA1_Channel1, ENABLE);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}
