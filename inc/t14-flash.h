/*
 * t14-flash.h
 *
 *  Created on: 25 окт. 2017 г.
 *      Author: Dima
 */

#ifndef T14_FLASH_H_
#define T14_FLASH_H_

#include <stdint.h>
#include "stm32l1xx.h"
#include "stm32.h"

FLASH_Status FLASH_Write_DataWord(uint32_t address, uint32_t data);

uint32_t FLASH_Read_DataWord(uint32_t address);



#endif /* T14_FLASH_H_ */
