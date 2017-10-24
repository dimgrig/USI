/*
 * FT_app.c
 *
 *  Created on: 21 окт. 2017 г.
 *      Author: Dima
 */

#include "FT_Platform.h"
#include "FT_App.h"
#include "arial.TTF_18_L1.rawh"

extern FT_PROGMEM ft_prog_uchar8_t SAMApp_Metric_L1[];
extern FT_PROGMEM ft_prog_uchar8_t SAMApp_L1[];
ft_uint32_t SAMApp_Metric_L1_SIZE = 148;
ft_uint32_t SAMApp_L1_SIZE = 6174;

static ft_uint8_t sk=0;
ft_uint32_t Ft_CmdBuffer_Index;
ft_uint32_t Ft_DlBuffer_Index;

#ifdef BUFFER_OPTIMIZATION
ft_uint8_t  Ft_DlBuffer[FT_DL_SIZE];
ft_uint8_t  Ft_CmdBuffer[FT_CMD_FIFO_SIZE];
#endif

ft_void_t Ft_App_WrCoCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd)
{
#ifdef  BUFFER_OPTIMIZATION
   /* Copy the command instruction into buffer */
   ft_uint32_t *pBuffcmd;
   pBuffcmd =(ft_uint32_t*)&Ft_CmdBuffer[Ft_CmdBuffer_Index];
   *pBuffcmd = cmd;
#endif
   Ft_Gpu_Hal_WrCmd32(phost,cmd);

#ifdef ARDUINO_PLATFORM
   Ft_Gpu_Hal_WrCmd32(phost,cmd);
#endif

   /* Increment the command index */
   Ft_CmdBuffer_Index += FT_CMD_SIZE;
}

ft_void_t Ft_App_WrDlCmd_Buffer(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t cmd)
{
#ifdef BUFFER_OPTIMIZATION
   /* Copy the command instruction into buffer */
   ft_uint32_t *pBuffcmd;
   pBuffcmd =(ft_uint32_t*)&Ft_DlBuffer[Ft_DlBuffer_Index];
   *pBuffcmd = cmd;
#endif
   Ft_Gpu_Hal_Wr32(phost,(RAM_DL+Ft_DlBuffer_Index),cmd);
#ifdef ARDUINO_PLATFORM
   Ft_Gpu_Hal_Wr32(phost,(RAM_DL+Ft_DlBuffer_Index),cmd);
#endif
   /* Increment the command index */
   Ft_DlBuffer_Index += FT_CMD_SIZE;
}

ft_void_t Ft_App_WrCoStr_Buffer(Ft_Gpu_Hal_Context_t *phost,const ft_char8_t *s)
{
#ifdef  BUFFER_OPTIMIZATION
  ft_uint16_t length = 0;
  length = strlen(s) + 1;//last for the null termination

  strcpy(&Ft_CmdBuffer[Ft_CmdBuffer_Index],s);

  /* increment the length and align it by 4 bytes */
  Ft_CmdBuffer_Index += ((length + 3) & ~3);
#endif
}

ft_void_t Ft_App_Flush_DL_Buffer(Ft_Gpu_Hal_Context_t *phost)
{
#ifdef  BUFFER_OPTIMIZATION
   if (Ft_DlBuffer_Index > 0)
     Ft_Gpu_Hal_WrMem(phost,RAM_DL,Ft_DlBuffer,Ft_DlBuffer_Index);
#endif
   Ft_DlBuffer_Index = 0;

}

ft_void_t Ft_App_Flush_Co_Buffer(Ft_Gpu_Hal_Context_t *phost)
{
#ifdef  BUFFER_OPTIMIZATION
   if (Ft_CmdBuffer_Index > 0)
     Ft_Gpu_Hal_WrCmdBuf(phost,Ft_CmdBuffer,Ft_CmdBuffer_Index);
#endif
   Ft_CmdBuffer_Index = 0;
}

ft_void_t SAMAPP_BootupConfig(Ft_Gpu_Hal_Context_t *phost)
{
	/* Do a power cycle for safer side */
	Ft_Gpu_Hal_Powercycle(phost,FT_TRUE);

	/* Access address 0 to wake up the FT800 */
	Ft_Gpu_HostCommand(phost,FT_GPU_ACTIVE_M);
	Ft_Gpu_Hal_Sleep(20);

	/* Set the clk to external clock */
	Ft_Gpu_HostCommand(phost,FT_GPU_EXTERNAL_OSC);
	Ft_Gpu_Hal_Sleep(20);


	/* Switch PLL output to 48MHz */
	Ft_Gpu_HostCommand(phost,FT_GPU_PLL_48M);
	Ft_Gpu_Hal_Sleep(20);

	/* Do a core reset for safer side */
	Ft_Gpu_HostCommand(phost,FT_GPU_CORE_RESET);


	{
		ft_uint8_t chipid;
		//Read Register ID to check if FT800 is ready.
		Ft_Gpu_Hal_Sleep(100);
		chipid = Ft_Gpu_Hal_Rd8(phost, REG_ID);
		Ft_Gpu_Hal_Sleep(100);
		while(chipid != 0x7C)
		chipid = Ft_Gpu_Hal_Rd8(phost, REG_ID);

	}
	 /* Global variables for display resolution to support various display panels */
	  /* Default is WQVGA - 480x272 */
	  ft_int16_t FT_DispWidth = 320;
	  ft_int16_t FT_DispHeight = 240;
	  ft_int16_t FT_DispHCycle =  360;
	  ft_int16_t FT_DispHOffset = 43;
	  ft_int16_t FT_DispHSync0 = 0;
	  ft_int16_t FT_DispHSync1 = 63;
	  ft_int16_t FT_DispVCycle = 278;
	  ft_int16_t FT_DispVOffset = 12;
	  ft_int16_t FT_DispVSync0 = 0;
	  ft_int16_t FT_DispVSync1 = 15;
	  ft_uint8_t FT_DispPCLK = 8;
	  ft_char8_t FT_DispSwizzle = 0;
	  ft_char8_t FT_DispPCLKPol = 1;

	    FT_DispWidth = 320;
		FT_DispHeight = 240;
		FT_DispHCycle =  408;
		FT_DispHOffset = 70;
		FT_DispHSync0 = 0;
		FT_DispHSync1 = 10;
		FT_DispVCycle = 263;
		FT_DispVOffset = 13;
		FT_DispVSync0 = 0;
		FT_DispVSync1 = 2;
		FT_DispPCLK = 8;
		FT_DispSwizzle = 2;
		FT_DispPCLKPol = 0;


	//	FT_DispWidth = 320;
	//	FT_DispHeight = 240;
	//	FT_DispHCycle =  400;
	//	FT_DispHOffset = 70;
	//	FT_DispHSync0 = 0;
	//	FT_DispHSync1 = 10;
	//	FT_DispVCycle = 250;
	//	FT_DispVOffset = 5;
	//	FT_DispVSync0 = 0;
	//	FT_DispVSync1 = 2;
	//	FT_DispPCLK = 8;
	//	FT_DispSwizzle = 2;
	//	FT_DispPCLKPol = 0;


	  Ft_Gpu_Hal_Wr16(phost, REG_HCYCLE, FT_DispHCycle);
	  Ft_Gpu_Hal_Wr16(phost, REG_HOFFSET, FT_DispHOffset);
	  Ft_Gpu_Hal_Wr16(phost, REG_HSYNC0, FT_DispHSync0);
	  Ft_Gpu_Hal_Wr16(phost, REG_HSYNC1, FT_DispHSync1);
	  Ft_Gpu_Hal_Wr16(phost, REG_VCYCLE, FT_DispVCycle);
	  Ft_Gpu_Hal_Wr16(phost, REG_VOFFSET, FT_DispVOffset);
	  Ft_Gpu_Hal_Wr16(phost, REG_VSYNC0, FT_DispVSync0);
	  Ft_Gpu_Hal_Wr16(phost, REG_VSYNC1, FT_DispVSync1);
	  Ft_Gpu_Hal_Wr8(phost, REG_SWIZZLE, FT_DispSwizzle);
	  Ft_Gpu_Hal_Wr8(phost, REG_PCLK_POL, FT_DispPCLKPol);
	  Ft_Gpu_Hal_Sleep(100);
	  Ft_Gpu_Hal_Wr8(phost, REG_PCLK,FT_DispPCLK);//after this display is visible on the LCD
	  Ft_Gpu_Hal_Wr16(phost, REG_HSIZE, FT_DispWidth);
	  Ft_Gpu_Hal_Wr16(phost, REG_VSIZE, FT_DispHeight);

	  Ft_Gpu_Hal_Wr8(phost, REG_GPIO_DIR,0x83 | Ft_Gpu_Hal_Rd8(phost,REG_GPIO_DIR));
	  Ft_Gpu_Hal_Wr8(phost, REG_GPIO,0x083 | Ft_Gpu_Hal_Rd8(phost,REG_GPIO));


	/* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
	  Ft_Gpu_Hal_Wr16(phost, REG_TOUCH_RZTHRESH,1200);
	  Ft_Gpu_Hal_Wr8(phost, REG_GPIO_DIR,0xff);
	  Ft_Gpu_Hal_Wr8(phost, REG_GPIO,0x0ff);

//	  Ft_Gpu_Hal_Wr16(phost, REG_TOUCH_RZTHRESH,1200);
//	delay_ms(10);
//	Ft_Gpu_Hal_Wr32(phost, REG_PWM_DUTY,255);


	 /* Boot up for FT800 followed by graphics primitive sample cases */
	  /* Initial boot up DL - make the back ground green color */
	  const ft_uint8_t FT_DLCODE_BOOTUP[12] =
	  {
	      0,255,0,2,//GPU instruction CLEAR_COLOR_RGB
	      7,0,0,38, //GPU instruction CLEAR
	      0,0,0,0,  //GPU instruction DISPLAY
	  };

	  /*It is optional to clear the screen here*/
//	      Ft_Gpu_Hal_WrMem(phost, RAM_DL,(ft_uint8_t *)FT_DLCODE_BOOTUP,sizeof(FT_DLCODE_BOOTUP));
//	      Ft_Gpu_Hal_Wr8(phost, REG_DLSWAP,DLSWAP_FRAME);


//	      Ft_Gpu_Hal_Wr8(phost, REG_PCLK,FT_DispPCLK);//after this display is visible on the LCD

	  SAMAPP_API_Calibrate(phost, 0); // работает

	  Ft_Gpu_Hal_Sleep(50);//Show the booting up screen.

}

ft_void_t SAMAPP_API_Calibrate(Ft_Gpu_Hal_Context_t *phost, ft_uint8_t mode)
{
	ft_uint32_t REG_TOUCH_CALIBRATE_A;
	ft_uint32_t REG_TOUCH_CALIBRATE_B;
	ft_uint32_t REG_TOUCH_CALIBRATE_C;
	ft_uint32_t REG_TOUCH_CALIBRATE_D;
	ft_uint32_t REG_TOUCH_CALIBRATE_E;
	ft_uint32_t REG_TOUCH_CALIBRATE_F;
	if (mode == 0) {
		REG_TOUCH_CALIBRATE_A = 0x00005a81;
		REG_TOUCH_CALIBRATE_B = 0x00000066;
		REG_TOUCH_CALIBRATE_C = 0xfff89d6b;
		REG_TOUCH_CALIBRATE_D = 0xfffffcb6;
		REG_TOUCH_CALIBRATE_E = 0x00006c3f;
		REG_TOUCH_CALIBRATE_F = 0x20003fd0;

		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_A, REG_TOUCH_CALIBRATE_A);
		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_B, REG_TOUCH_CALIBRATE_B);
		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_C, REG_TOUCH_CALIBRATE_C);
		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_D, REG_TOUCH_CALIBRATE_D);
		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_E, REG_TOUCH_CALIBRATE_E);
		Ft_Gpu_Hal_Wr32(phost, REG_TOUCH_TRANSFORM_F, REG_TOUCH_CALIBRATE_F);
	} else {
		Ft_Gpu_CoCmd_Dlstart(phost);
		//Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0,0,0));
		Ft_Gpu_CoCmd_Calibrate(phost,0);
		Ft_App_Flush_Co_Buffer(phost);
		Ft_Gpu_Hal_WaitCmdfifo_empty(phost);

		REG_TOUCH_CALIBRATE_A = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_A);
		REG_TOUCH_CALIBRATE_B = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_B);
		REG_TOUCH_CALIBRATE_C = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_C);
		REG_TOUCH_CALIBRATE_D = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_D);
		REG_TOUCH_CALIBRATE_E = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_E);
		REG_TOUCH_CALIBRATE_F = Ft_Gpu_Hal_Rd32(phost, REG_TOUCH_TRANSFORM_F);
	}

    Ft_Gpu_Hal_Sleep(50);
}

ft_void_t SAMAPP_API_Screen(Ft_Gpu_Hal_Context_t *phost, ft_char8_t *str)
{

	Ft_Gpu_CoCmd_Dlstart(phost);
	Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0,0,0));
	Ft_App_WrCoCmd_Buffer(phost,CLEAR(0xff,0xff,0xff));

	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0xff,0x00,0x00));
	Ft_Gpu_CoCmd_Text(phost, 150, 100, 16, OPT_CENTER, str);

	Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
	Ft_Gpu_CoCmd_Swap(phost);

	/* Download the commands into fifo */
	Ft_App_Flush_Co_Buffer(phost);

	/* Wait till coprocessor completes the operation */
	Ft_Gpu_Hal_WaitCmdfifo_empty(phost);

    Ft_Gpu_Hal_Sleep(50);
}

ft_uint16_t API_Screen_BackGround(Ft_Gpu_Hal_Context_t *phost, ft_char8_t *str)
{
	  Ft_Gpu_Hal_WrMemFromFlash(phost, RAM_G + 1000, SAMApp_Metric_L1, SAMApp_Metric_L1_SIZE); //загружаем метрику шрифта
	  Ft_Gpu_Hal_WrMemFromFlash(phost, RAM_G + 1000 + SAMApp_Metric_L1_SIZE, SAMApp_L1, SAMApp_L1_SIZE); //загружаем шрифт

	  Ft_Gpu_CoCmd_Dlstart(phost); // команда начала дисплей-листа



	  //Ft_Gpu_CoCmd_Dlstart(phost);
	  	Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0,0,0));
	  	Ft_App_WrCoCmd_Buffer(phost,CLEAR(0xff,0xff,0xff));


	  //Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0xff,0xff,0xff)); //установка цвета фона
	  Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1)); //установка цвета фона
	  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,0,0)); // установка цвета текста
	  Ft_App_WrCoCmd_Buffer(phost,BITMAP_HANDLE(12)); //назначение нашему шрифту указателя

	  Ft_App_WrCoCmd_Buffer(phost,BITMAP_SOURCE(1085));
	  Ft_App_WrCoCmd_Buffer(phost, BITMAP_LAYOUT(L1,3,21));
	  Ft_App_WrCoCmd_Buffer(phost, BITMAP_SIZE(NEAREST, BORDER,BORDER,18,21));
	  Ft_Gpu_CoCmd_SetFont(phost, 12, RAM_G + 1000);



	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0x00,0xff,0x00));
	Ft_Gpu_CoCmd_Text(phost, 10, 10, 12, 0, "\x21\x24\x14\x06\x0c\x28\x43\x5b\x5d");
	//Ft_Gpu_CoCmd_Text(phost, 10, 10, 16, 0, str);
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0xff,0x00,0x00));
	Ft_Gpu_CoCmd_Text(phost, 10, 30, 12, 0, "\x58\x59\x5a");
	//Ft_Gpu_CoCmd_Text(phost, 10, 30, 16, 0, 777);

	Ft_App_WrCoCmd_Buffer(phost,BEGIN(LINES));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(0, 30, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(300, 30, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(0, 50, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(300, 50, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(160, 0, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(160, 120, 0, 0));
	Ft_App_WrCoCmd_Buffer(phost,END());


	Ft_App_WrCoCmd_Buffer(phost,TAG(1));          // assign the tag value
	Ft_Gpu_CoCmd_Button(phost, 10, 180, 90, 40, 18, 0, "START");
	Ft_App_WrCoCmd_Buffer(phost,TAG_MASK(0));

	Ft_App_WrCoCmd_Buffer(phost,TAG_MASK(1));
	Ft_App_WrCoCmd_Buffer(phost,TAG(2));          // assign the tag value
	Ft_Gpu_CoCmd_Button(phost, 110, 180, 90, 40, 18, 0, "STOP");
	Ft_App_WrCoCmd_Buffer(phost,TAG_MASK(0));

	//Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
	//Ft_Gpu_CoCmd_Swap(phost);

	Ft_App_Flush_Co_Buffer(phost);
	Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
	//while(0!=Ft_Gpu_Hal_Rd16(phost,REG_CMD_READ));

	ft_uint16_t dloffset = Ft_Gpu_Hal_Rd16(phost,REG_CMD_DL); // размер коируемого дисплей-листа
	//dloffset -= 4;
	Ft_Gpu_Hal_WrCmd32(phost,CMD_MEMCPY); // указание FT800 скопировать области памяти
	Ft_Gpu_Hal_WrCmd32(phost,100000L); // адрес, куда будем копировать в области графической памяти RAM_G
	Ft_Gpu_Hal_WrCmd32(phost,RAM_DL); // адрес, откуда копируем
	Ft_Gpu_Hal_WrCmd32(phost,dloffset); // количество байт



		/* Download the commands into fifo */
		//Ft_App_Flush_Co_Buffer(phost);

		/* Wait till coprocessor completes the operation */
		//Ft_Gpu_Hal_WaitCmdfifo_empty(phost);

	Ft_Gpu_Hal_Sleep(50);
	return dloffset;

}

ft_void_t API_Screen_Content(Ft_Gpu_Hal_Context_t *phost, ft_uint16_t dloffset, ft_uint16_t nmb)
{
	Ft_Gpu_CoCmd_Dlstart(phost); // начало нового дисплей-листа
	Ft_Gpu_CoCmd_Append(phost,100000L, dloffset); // добавление к текущему набору команд, команд скопированных MEMCPY

	//Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0,0,0));
	//Ft_App_WrCoCmd_Buffer(phost,CLEAR(0xff,0xff,0xff));

	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0x00,0x00,0xff));
	Ft_Gpu_CoCmd_Text(phost, 10, 50, 16, 0, "CONTENT");
	Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0xff,0xd8,0x00));
	Ft_Gpu_CoCmd_Number(phost, 10, 60, 16, 0, nmb);

	Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
	Ft_Gpu_CoCmd_Swap(phost);

	/* Download the commands into fifo */
	Ft_App_Flush_Co_Buffer(phost);

	/* Wait till coprocessor completes the operation */
	Ft_Gpu_Hal_WaitCmdfifo_empty(phost);

    Ft_Gpu_Hal_Sleep(50);
}



ft_uint8_t Read_Keys(Ft_Gpu_Hal_Context_t *phost)
{
  static ft_uint8_t Read_tag=0,temp_tag=0,ret_tag=0;
  Read_tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
  ret_tag = NULL;
  if(Read_tag!=NULL)								// Allow if the Key is released
  {
    if(temp_tag!=Read_tag)
    {
      temp_tag = Read_tag;
      sk = Read_tag;										// Load the Read tag to temp variable
    }
  }
  else
  {
    if(temp_tag!=0)
    {
      ret_tag = temp_tag;
    }
    sk = 0;
  }
  return ret_tag;
}



// Info Screen//
ft_void_t Info(Ft_Gpu_Hal_Context_t *phost)
{
	 ft_uint16_t dloffset = 0,z;
	  Ft_CmdBuffer_Index = 0;
	  ft_uint16_t FT_DispWidth = 320;
	  ft_uint16_t FT_DispHeight = 240;
	// Touch Screen Calibration
	  char *info[] = {  "FT800 Sketch Application",
					   "APP to demonstrate interactive Sketch,",
					   "using Sketch, Slider,",
					   "& Buttons."
	 	  	                    };

	  Ft_Gpu_CoCmd_Dlstart(phost);
	  Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1));
	  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,255,255));
	  Ft_Gpu_CoCmd_Text(phost,10,150,18,0,"Please tap on a dot");
//	  Ft_Gpu_CoCmd_Calibrate(phost,0);
//	  Ft_App_Flush_Co_Buffer(phost);
//	  Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
	// Ftdi Logo animation
//	  Ft_Gpu_CoCmd_Logo(phost);
	  Ft_App_Flush_Co_Buffer(phost);
	  Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
	  //while(0!=Ft_Gpu_Hal_Rd16(phost,REG_CMD_READ));
	  dloffset = Ft_Gpu_Hal_Rd16(phost,REG_CMD_DL);
	  //dloffset -= 4;

	#ifdef FT_81X_ENABLE
	  dloffset -= 2*4;//remove two more instructions in case of 81x
	#endif


	  Ft_Gpu_Hal_WrCmd32(phost,CMD_MEMCPY);
	  Ft_Gpu_Hal_WrCmd32(phost,100000L);
	  Ft_Gpu_Hal_WrCmd32(phost,RAM_DL);
	  Ft_Gpu_Hal_WrCmd32(phost,dloffset);

	  Ft_Gpu_Hal_Sleep(500);
	  //Enter into Info Screen
	  do
	  {
	    Ft_Gpu_CoCmd_Dlstart(phost);
	    Ft_Gpu_CoCmd_Append(phost,100000L,dloffset);
	    //Reset the BITMAP properties used during Logo animation
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_A(256));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_A(256));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_B(0));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_C(0));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_D(0));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_E(256));
//	    Ft_App_WrCoCmd_Buffer(phost,BITMAP_TRANSFORM_F(0));
//	    Ft_App_WrCoCmd_Buffer(phost,SAVE_CONTEXT());
	    //Display the information with transparent Logo using Edge Strip
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(219,180,150));
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_A(220));
//	    Ft_App_WrCoCmd_Buffer(phost,BEGIN(EDGE_STRIP_A));
//	    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(0,FT_DispHeight*16));
//	    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(FT_DispWidth*16,FT_DispHeight*16));
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_A(255));
//	    Ft_App_WrCoCmd_Buffer(phost,RESTORE_CONTEXT());
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0,0,0));
	   // INFORMATION

	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0x00,0x00,0xff));
	    Ft_Gpu_CoCmd_Text(phost,10,20,18,0,"1");
	    Ft_Gpu_CoCmd_Text(phost,10,60,18,0,"2");
	    Ft_Gpu_CoCmd_Text(phost,10,90,18,0,"3");
	    Ft_Gpu_CoCmd_Text(phost,10,120,18,0,"4");
	    Ft_Gpu_CoCmd_Text(phost,10,200,18,0,"Click to play");
//	    if(sk!='P')
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,255,255));
//	    else
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(100,100,100));
//	    Ft_App_WrCoCmd_Buffer(phost,BEGIN(FTPOINTS));
//	    Ft_App_WrCoCmd_Buffer(phost,POINT_SIZE(20*16));
//	    Ft_App_WrCoCmd_Buffer(phost,TAG('P'));
//	    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F((FT_DispWidth/2)*16,(FT_DispHeight-60)*16));
//	    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(180,35,35));
//	    Ft_App_WrCoCmd_Buffer(phost,BEGIN(BITMAPS));


//	#ifdef DISPLAY_RESOLUTION_WVGA
//	    Ft_App_WrCoCmd_Buffer(phost, BITMAP_HANDLE(14));
//	    Ft_App_WrCoCmd_Buffer(phost, CELL(4));
//	    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F((FT_DispWidth/2-14)*16,(FT_DispHeight-75)*16));
//	#else
//	    Ft_App_WrCoCmd_Buffer(phost,VERTEX2II((FT_DispWidth/2)-14,(FT_DispHeight-75),14,4));
//	#endif
//	    Ft_App_WrCoCmd_Buffer(phost,END());

	    Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
	    Ft_Gpu_CoCmd_Swap(phost);
	    Ft_App_Flush_Co_Buffer(phost);
	    Ft_Gpu_Hal_WaitCmdfifo_empty(phost);

	    Ft_Gpu_Hal_Sleep(50);
	  }while(Read_Keys(phost)!='P');
	  //Ft_Play_Sound(0x50,255,0xc0);
}

/* Application*/
ft_void_t Sketch(Ft_Gpu_Hal_Context_t *phost)
{
  ft_uint32_t  tracker,color=0;
  ft_uint16_t  val=32768;
  ft_uint8_t tag =0;
  ft_uint16_t FT_DispWidth = 320;
  	  ft_uint16_t FT_DispHeight = 240;
//  Set the bitmap properties , sketch properties and Tracker for the sliders
  Ft_Gpu_CoCmd_Dlstart(phost);
  Ft_Gpu_CoCmd_FgColor(phost,0xffffff);        // Set the bg color
  Ft_Gpu_CoCmd_Track(phost,(320-30),40,8,280-100,1);

#if defined FT_801_ENABLE
  Ft_Gpu_CoCmd_CSketch(phost,0,10,320-40,280-30,0,L8,1500L);
#elif defined FT_81X_ENABLE
  Ft_Gpu_CoCmd_Sketch(phost,0,10,FT_DispWidth-40,FT_DispHeight-30,0,L8);
#else
  Ft_Gpu_CoCmd_Sketch(phost,0,10,FT_DispWidth-40,FT_DispHeight-30,0,L8);
#endif
/*
#if defined FT_801_ENABLE
  Ft_Gpu_CoCmd_CSketch(phost,0,10,FT_DispWidth-40,FT_DispHeight-20,0,L8,1500L);
#elif defined FT_81X_ENABLE
  Ft_Gpu_CoCmd_Sketch(phost,0,10,FT_DispWidth-40,FT_DispHeight-20,0,L8);
#else
  Ft_Gpu_CoCmd_Sketch(phost,0,10,FT_DispWidth-40,FT_DispHeight-20,0,L8);
#endif
  */
  Ft_Gpu_CoCmd_MemZero(phost,0L,(FT_DispWidth-40)*(FT_DispHeight-20L));
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_HANDLE(1));
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_SOURCE(0));
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_LAYOUT(L8,FT_DispWidth-40,FT_DispHeight-20));
#ifdef FT_81X_ENABLE
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_LAYOUT_H((FT_DispWidth-40)>>10,(FT_DispHeight-20)>>9));
#endif
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_SIZE(NEAREST,BORDER,BORDER,(FT_DispWidth-40),(FT_DispHeight-20)));
#ifdef FT_81X_ENABLE
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_SIZE_H((FT_DispWidth-40)>>9,(FT_DispHeight-20)>>9));
#endif
  Ft_Gpu_CoCmd_Swap(phost);
  Ft_App_Flush_Co_Buffer(phost);
  Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
  while(1)
  {
    // Check the tracker
    tracker = Ft_Gpu_Hal_Rd32(phost,REG_TRACKER);
        // Check the Tag
    tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
    //  clear the GRAM when user enter the Clear button
    if(tag==2)
    {
  	Ft_Gpu_CoCmd_Dlstart(phost);
 	Ft_Gpu_CoCmd_MemZero(phost,0,(FT_DispWidth-40)*(FT_DispHeight-20L)); // Clear the gram frm 1024
	Ft_App_Flush_Co_Buffer(phost);
        Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
    }
    // compute the color from the tracker
    if((tracker&0xff)==1)      // check the tag val
    {
      val = (tracker>>16);
    }
    color = val*255;
    // Start the new display list
    Ft_Gpu_CoCmd_Dlstart(phost);                  // Start the display list
    Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1));	  // clear the display
    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,255,255));  // color
    Ft_Gpu_CoCmd_BgColor(phost,color);
    Ft_App_WrCoCmd_Buffer(phost,TAG_MASK(1));
    Ft_App_WrCoCmd_Buffer(phost,TAG(1));          // assign the tag value
    Ft_Gpu_CoCmd_FgColor(phost,color);
   // draw the sliders
    Ft_Gpu_CoCmd_Slider(phost,(FT_DispWidth-30),40,8,(FT_DispHeight-100),0,val,65535);	 // slide j1 cmd
    Ft_Gpu_CoCmd_FgColor(phost,(tag==2)?0x0000ff:color);
    Ft_App_WrCoCmd_Buffer(phost,TAG(2));          // assign the tag value
    Ft_Gpu_CoCmd_Button(phost,(FT_DispWidth-35),(FT_DispHeight-45),35,25,26,0,"CLR");
    Ft_App_WrCoCmd_Buffer(phost,TAG_MASK(0));

    Ft_Gpu_CoCmd_Text(phost,FT_DispWidth-35,10,26,0,"Color");

    Ft_App_WrCoCmd_Buffer(phost,LINE_WIDTH(1*16));
    Ft_App_WrCoCmd_Buffer(phost,BEGIN(RECTS));
    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(0,10*16));
    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F((ft_int16_t)(FT_DispWidth-40)*16,(ft_int16_t)(FT_DispHeight-20)*16));


    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB((color>>16)&0xff,(color>>8)&0xff,(color)&0xff));
    Ft_App_WrCoCmd_Buffer(phost,BEGIN(BITMAPS));
    Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(0,10,1,0));
    Ft_App_WrCoCmd_Buffer(phost,END());
    Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
    Ft_Gpu_CoCmd_Swap(phost);
    Ft_App_Flush_Co_Buffer(phost);
    Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
  }
}

ft_void_t SAMAPP_Russian(Ft_Gpu_Hal_Context_t *phost)
{
  Ft_Gpu_Hal_WrMemFromFlash(phost, RAM_G + 1000, SAMApp_Metric_L1, SAMApp_Metric_L1_SIZE); //загружаем метрику шрифта
  Ft_Gpu_Hal_WrMemFromFlash(phost, RAM_G + 1000 + SAMApp_Metric_L1_SIZE, SAMApp_L1, SAMApp_L1_SIZE); //загружаем шрифт

  Ft_Gpu_CoCmd_Dlstart(phost); // команда начала дисплей-листа

  Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(0xff,0xff,0xff)); //установка цвета фона
  Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1)); //установка цвета фона
  Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(255,0,0)); // установка цвета текста
  Ft_App_WrCoCmd_Buffer(phost,BITMAP_HANDLE(12)); //назначение нашему шрифту указателя

  Ft_App_WrCoCmd_Buffer(phost,BITMAP_SOURCE(1085));
  Ft_App_WrCoCmd_Buffer(phost, BITMAP_LAYOUT(L1,3,21));
  Ft_App_WrCoCmd_Buffer(phost, BITMAP_SIZE(NEAREST, BORDER,BORDER,18,21));
  Ft_Gpu_CoCmd_SetFont(phost, 12, RAM_G + 1000);

//  Ft_App_WrCoCmd_Buffer(phost,BEGIN(BITMAPS));
//  for(int i=1;i<=10;i++)
//  {
//   //show font 1~10 in low 1
//   Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(i*18,20,12,i));
//   //show font 11~20 in low 2
//   Ft_App_WrCoCmd_Buffer(phost,VERTEX2II(i*18,56,12,i+10));
//  }
//  Ft_App_WrCoCmd_Buffer(phost,END());

  Ft_Gpu_CoCmd_Text(phost, 10, 80, 12, 0, "\x11\x21\x24\x14\x06\x0c\x28\x43\x5b\x5d");

  Ft_App_WrCoCmd_Buffer(phost,DISPLAY());
  Ft_Gpu_CoCmd_Swap(phost);
  Ft_App_Flush_Co_Buffer(phost);
  Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
  Ft_Gpu_Hal_Sleep(300);
 }

