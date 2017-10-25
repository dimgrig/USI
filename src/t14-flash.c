/*
 * t14-flash.c
 *
 *  Created on: 25 окт. 2017 г.
 *      Author: Dima
 */
#include "t14-flash.h"
#include "stm32l1xx_flash.h"

FLASH_Status FLASH_Write_DataWord(uint32_t address, uint32_t data) {
  FLASH_Status status = FLASH_COMPLETE;
  address = address + 0x08080000;
  FLASH_UnlockData();  //Unprotect the EEPROM to allow writing
  status = FLASH_WriteDataWord(address, data);
  FLASH_LockData();  // Reprotect the EEPROM
  return status;
}

uint32_t FLASH_Read_DataWord(uint32_t address) {
  uint32_t tmp = 0;
  address = address + 0x08080000;
  tmp = *(__IO uint32_t*)address;
  return tmp;
}
