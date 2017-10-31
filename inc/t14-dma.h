/*
 * t14-dma.h
 *
 *  Created on: 28 окт. 2017 г.
 *      Author: Dima
 */

#ifndef T14_DMA_H_
#define T14_DMA_H_

#define DMA_BUFFER_SIZE 512

#include "stm32l1xx.h"
#include "stm32.h"

void DMA_init(void);


uint16_t DMA_BUFFER[DMA_BUFFER_SIZE];
//uint16_t DMA_BUFFER_[DMA_BUFFER_SIZE];

#endif /* T14_DMA_H_ */
