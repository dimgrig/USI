/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h"
#include "TouchPanel.h"

/* Private variables ---------------------------------------------------------*/
//Matrix matrix ;
#ifdef TP_CALIBRATE
Matrix matrix = {						
/*-290,  
-338865,
99546353,
475585,
-12165,    
-1743583495,     
-4939851,*/
  
7105,
-307890,
93718355,
468355,
14735,
-1652759995,
-4420317
};
#else
Matrix matrix;
#endif
Coordinate  display ;

/* LCD coordinates corresponding sampling AD value. for example£ºLCD coordinates 45, 45 X Y sampling ADC value 3388, 920 */	
Coordinate ScreenSample[3];
/* LCD Coordinate */
Coordinate DisplaySample[3] =   {
                                            { 45, 45 },
											{ 45, 270},
                                            { 190,190}
	                            } ;

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2   /* threshold */

/*******************************************************************************
* Function Name  : TP_Init
* Description    : ADS7843 SPI Initialization
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void) 
{ 
///	PINSEL_CFG_Type PinCfg;
///    SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.6  - TP_CS - used as GPIO
	 * P0.7  - TP_SCK
	 * P0.8  - TP_SDO
	 * P0.9  - TP_SDI
	 * P2.13 - TP_IRQ - used as GPIO
	 */
///	PinCfg.Funcnum = 2;
///	PinCfg.OpenDrain = 0;
///	PinCfg.Pinmode = 0;
///	PinCfg.Portnum = 0;
///	PinCfg.Pinnum = 7;
///	PINSEL_ConfigPin(&PinCfg);
///	PinCfg.Pinnum = 8;
///	PINSEL_ConfigPin(&PinCfg);
///	PinCfg.Pinnum = 9;
///	PINSEL_ConfigPin(&PinCfg);

///	PinCfg.Funcnum = 0;
///	PinCfg.Portnum = 0;
///	PinCfg.Pinnum = 6;
///	PINSEL_ConfigPin(&PinCfg);

///	PinCfg.Funcnum = 0;
///	PinCfg.Portnum = 2;
///	PinCfg.Pinnum = 13;
///	PINSEL_ConfigPin(&PinCfg);

    /* P2.13 TP_IRQ is Input */
///    GPIO_SetDir(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM), 0);
///	GPIO_SetValue(TP_IRQ_PORT_NUM, (1<<TP_IRQ_PIN_NUM));  
    /* P0.6 TP_CS is output */
///    GPIO_SetDir(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM), 1);
///	GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM)); 

	/* initialize SSP configuration structure to default */
///	SSP_ConfigStructInit(&SSP_ConfigStruct);
///	SSP_ConfigStruct.ClockRate = 500000;
///	/* Initialize SSP peripheral with parameter given in structure above */
///	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

///	/* Enable SSP peripheral */
///	SSP_Cmd(LPC_SSP1, ENABLE);
} 

/*******************************************************************************
* Function Name  : DelayUS
* Description    : delay 1us
* Input          : - cnt: delay value
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DelayUS( unsigned int cnt)
{
  unsigned int i;
  for(i = 0;i<cnt;i++)
  {
     unsigned char us = 12;
     while (us--)
     {
       ;   
     }
  }
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : write data to ADS7843
* Input          : - cmd: data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint8_t WR_CMD (uint8_t cmd)  
{ 
	/* wait for current SSP activity complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) ==  SET);

	SPI_I2S_SendData(SPI1, cmd);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint16_t)SPI_I2S_ReceiveData(SPI1);
} 

/*******************************************************************************
* Function Name  : RD_AD
* Description    : read adc value
* Input          : None
* Output         : None
* Return         : ADS7843 Return two byte data
* Attention		 : None
*******************************************************************************/
static int RD_AD(void)  
{ 
	unsigned short buf,temp; 
	
	temp = WR_CMD(0x00);

	buf = temp << 8; 
	DelayUS(1); 

	temp = WR_CMD(0x00);

	buf |= temp; 
	buf >>= 3; 
	buf &= 0xfff;

	return buf; 
} 

/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC value of X + channel 
* Input          : None
* Output         : None
* Return         : return X + channel ADC value
* Attention		 : None
*******************************************************************************/
int Read_X(void)  
{  
	int i; 
	SPI_TP_CS_LOW(); 
	DelayUS(1); 
	WR_CMD(CHX); 
	DelayUS(1); 
	i = RD_AD(); 
	SPI_TP_CS_HIGH(); 
	return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC value of Y + channel 
* Input          : None
* Output         : None
* Return         : return Y + channel ADC value
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
	int i; 
	SPI_TP_CS_LOW(); 
	DelayUS(1); 
	WR_CMD(CHY); 
	DelayUS(1); 
	i = RD_AD(); 
	SPI_TP_CS_HIGH(); 
	return i;     
} 

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843 ADC value of X + Y + channel 
* Input          : None
* Output         : None
* Return         : return X + Y + channel ADC value
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
	int adx,ady; 
	adx = Read_X(); 
	DelayUS(1); 
	ady = Read_Y(); 
	*x = adx; 
	*y = ady; 
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : Draw point Must have a LCD driver
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetPoint(Xpos,Ypos,0xf800);     /* Center point */
  LCD_SetPoint(Xpos+1,Ypos,0xf800);
  LCD_SetPoint(Xpos,Ypos+1,0xf800);
  LCD_SetPoint(Xpos+1,Ypos+1,0xf800);	
}

/*******************************************************************************
* Function Name  : DrawCross
* Description    : specified coordinates painting crosshairs
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
  LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,0xffff);
  
  LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,0xffff);
  
  LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,0xffff);
  
  LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,0xffff);
  
  LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,RGB565CONVERT(184,158,131));	  	
}	

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : X Y obtained after filtering
* Input          : None
* Output         : None
* Return         : Coordinate Structure address
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count = 0;
  int buffer[2][9] = {{0},{0}};  /* Multiple sampling coordinates X and Y */
  
  
  while (           GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) != 0)
  {;}
  
  do					       /* Loop sampling 9 times */
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count] = TP_X[0];  
	buffer[1][count] = TP_Y[0];
	count++;  
  }
  while( /***! TP_INT_IN&&***/ (count < 9) );  /* when user clicks on the touch screen, TP_INT_IN touchscreen interrupt pin will be set to low. */
  
  if( count == 9 )   /* Successful sampling 9, filtering */ 
  {  
    /* In order to reduce the amount of computation, were divided into three groups averaged */
    temp[0] = ( buffer[0][0] + buffer[0][1] + buffer[0][2] ) / 3;
	temp[1] = ( buffer[0][3] + buffer[0][4] + buffer[0][5] ) / 3;
	temp[2] = ( buffer[0][6] + buffer[0][7] + buffer[0][8] ) / 3;
	/* Calculate the three groups of data */
	m0 = temp[0] - temp[1];
	m1 = temp[1] - temp[2];
	m2 = temp[2] - temp[0];
	/* Absolute value of the above difference */
	m0 = m0 > 0 ? m0 : (-m0);
    m1 = m1 > 0 ? m1 : (-m1);
	m2 = m2 > 0 ? m2 : (-m2);
	/* Judge whether the absolute difference exceeds the difference between the threshold, If these three absolute difference exceeds the threshold, 
       The sampling point is judged as outliers, Discard sampling points */
	if( m0 > THRESHOLD  &&  m1 > THRESHOLD  &&  m2 > THRESHOLD ) 
	{
	    return 0;
	}
	/* Calculating their average value */ 
	if( m0 < m1 )
	{
	  if( m2 < m0 ) 
	  {  
	      screen.x = ( temp[0] + temp[2] ) / 2;
	  }
	  else 
	  {
 	      screen.x = ( temp[0] + temp[1] ) / 2;	
	  }
	}
	else if(m2<m1) 
	{
 	    screen.x = ( temp[0] + temp[2] ) / 2;
	}
	else 
	{
 	    screen.x = ( temp[1] + temp[2] ) / 2;
	}

	/* calculate the average value of Y */
    temp[0] = ( buffer[1][0] + buffer[1][1] + buffer[1][2] ) / 3;
	temp[1] = ( buffer[1][3] + buffer[1][4] + buffer[1][5] ) / 3;
	temp[2] = ( buffer[1][6] + buffer[1][7] + buffer[1][8] ) / 3;
	
	m0 = temp[0] - temp[1];
	m1 = temp[1] - temp[2];
	m2 = temp[2] - temp[0];
	
	m0 = m0 > 0 ? m0 : (-m0);
	m1 = m1 > 0 ? m1 : (-m1);
	m2 = m2 > 0 ? m2 : (-m2);
	if( m0 > THRESHOLD && m1 > THRESHOLD && m2 > THRESHOLD )
	{
	    return 0;
	}

	if( m0 < m1 )
	{
	  if( m2 < m0 ) 
	  {
	      screen.y = ( temp[0] + temp[2] ) / 2;
	  }
	  else 
	  {
    	  screen.y = ( temp[0] + temp[1] ) / 2;
      }		  
    }
	else if( m2 < m1 ) 
	{
	    screen.y = ( temp[0] + temp[2] ) / 2;
	}
	else
	{
    	screen.y = ( temp[1] + temp[2] ) / 2;
    }
	return &screen;
  }
  
  return 0; 
}
	 
/* Here is the transfer function of the touch screen to the LCD screen coordinate transform */
/* Only in the error angle between the LCD and touch screen is very small, in order to use the following formula */

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculated K A B C D E F
* Input          : None
* Output         : None
* Return         : return 1 success , return 0 fail
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : channel XY via K A B C D E F value converted to the LCD screen coordinates
* Input          : None
* Output         : None
* Return         : return 1 success , return 0 fail
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : calibrate touch screen
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
  uint8_t i;
  Coordinate * Ptr;

  for(i=0;i<3;i++)
  {
      LCD_Clear(Black);
      GUI_Text(10,10,"Touch crosshair to calibrate", sizeof("Touch crosshair to calibrate"), 0xffff,Black);   
      DelayUS( 500 * 1000 );
      DrawCross(DisplaySample[i].x,DisplaySample[i].y);
      do
      {
		   Ptr = Read_Ads7846();
      }
      while( Ptr == (void*)0 );
   
      ScreenSample[i].x = Ptr->x; ScreenSample[i].y = Ptr->y;
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix ) ;  /* get calibration parameters */	   
  LCD_Clear(Black);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
