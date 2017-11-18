/*
 * FT_App.h
 *
 *  Created on: 21 окт. 2017 г.
 *      Author: Dima
 */
#include <stdio.h>
#include "FT_Platform.h"
#include "arial.TTF_18_L1.rawh"
#include "GUI.h"
#include "t14-flash.h"

#ifndef FT_APP_H_
#define FT_APP_H_

ft_void_t Ft_App_WrCoCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd);
ft_void_t Ft_App_WrDlCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd);
ft_void_t Ft_App_WrCoStr_Buffer(Ft_Gpu_Hal_Context_t *phost,const ft_char8_t *s);
ft_void_t Ft_App_Flush_DL_Buffer(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_App_Flush_Co_Buffer(Ft_Gpu_Hal_Context_t *phost);

ft_uint32_t white = COLOR_RGB(0xbb,0xbb,0xbb);
ft_uint32_t gray = COLOR_RGB(0x77,0x77,0x77);
ft_uint32_t button_color = COLOR_RGB(0x00,0x26,0x77);
ft_uint32_t button_color_hover = COLOR_RGB(0x00,0x55,0xff);
ft_uint32_t button_color_selected = COLOR_RGB(0x00,0xff,0x55);

ft_uint16_t FT_Width = 320;
ft_uint16_t FT_Height = 240;

ft_uint8_t DispText[20];
ft_uint8_t CurrChar = '|';
ft_uint8_t CurrTag = 0;
ft_uint8_t PrevTag = 0;
//ft_uint8_t Pendown = 1;
ft_uint8_t CurrTextIdx = 0;

#endif /* FT_APP_H_ */
