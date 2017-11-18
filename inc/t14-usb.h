/*
 * t14-usb.h
 *
 *  Created on: 30 окт. 2017 г.
 *      Author: Dima
 */

#ifndef T14_USB_H_
#define T14_USB_H_

#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32.h"


void RCC_Initializatiion(void);
void USB_LP_IRQHandler (void);
void USB_SetLeds(uint8_t);
void USB_Send_Packet(uint8_t *packet, uint16_t length);
//void USB_Send_State(State_TypeDef STATE);
uint8_t hextoascii(uint8_t hex);

#endif /* T14_USB_H_ */
