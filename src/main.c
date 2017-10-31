#include "stm32l1xx.h"
#include "stm32.h"
//#include "t14-uart.h"
#include "t14-spi.h"
#include "t14-math.h"
#include "GLCD.h"
#include "TouchPanel.h"
//#include "ESP8266.h"
#include "t14-timer.h"
//#include "SMSD.h"
#include "t14-adc.h"
#include "t14-dma.h"
#include "t14-ssi.h"
#include "t14-power.h"
#include "t14-flash.h"
#include "GUI.h"


#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "t14-usb.h"


#include "FT_Platform.h"
#include "SampleApp.h"

//#define TP_TEST_DRAW

volatile static uint32_t s_Tick;
void LEDsSet (unsigned int);

#define SPS 5000UL
#define Trc 0.001f
#define K (SPS*Trc)

static uint16_t Dacc = 0;
static uint8_t Dout = 0;


uint16_t init_finished = 0;

Screen_TypeDef SCREEN = MAIN;
ft_uint16_t dloffset;

State_TypeDef STATE = IDLE;
uint16_t TouchDelay = 0;

double F = 0; //текущие
double A = 0;
double E, HB, ST, SB;

double F1 = 0; //измерение
double A0 = 0; //касание
double A1 = 0; //измерение
double H = 0; //снятие
/*************************************************************************
 * Function Name: LEDsSet
 * Parameters: unsigned int State
 *
 * Return: none
 *
 * Description: Set LEDS State
 *
 *************************************************************************/
void LEDsSet (unsigned int State)
{

  if(State & 1)
  {
    STM32_LEDOn(LED1);
  }
  else
  {
    STM32_LEDOff(LED1);
  }
  if(State & 2)
  {
    STM32_LEDOn(LED2);
  }
  else
  {
    STM32_LEDOff(LED2);
  }
}



/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
  __disable_irq();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  TIMERS_init();
//  USART_init();
//  UART_rx_buffer_clear();

  SPI1_init(SPI_BaudRatePrescaler_16);
  SPI2_init();

  GPIO_SPI_init();
  SPI_TP_CS_HIGH();
  SPI_LCD_CS_HIGH();

  DMA_init();
  ADC_init();

  InitSSI();

//  LCD_Initializtion();
//  LCD_Clear(Black);

  #ifndef TP_CALIBRATE
    TouchPanel_Calibrate();
  #endif

//  UART_rx_buffer_clear();
//  ESP8266_init();

  LEDsSet(0x3);

//  placeMainScreen();
//  placeState(STATE);
//  placeReset();

  InitPower();
  EN_3V3_HIGH();
  EN_5V_HIGH();


  uint8_t i;
  RCC_Initializatiion();
  SYSCFG_USBPuCmd( ENABLE );

  USB_Interrupts_Config();
  USB_Init();





  Ft_Gpu_HalInit_t halinit;
  Ft_Gpu_Hal_Context_t host,*phost;

  halinit.TotalChannelNum = 1;

  Ft_Gpu_Hal_Init(&halinit);
  host.hal_config.channel_no = 0;
  host.hal_config.spi_clockrate_khz = 2000; //in KHz

  Ft_Gpu_Hal_Open(&host);
  phost = &host;

  Ft_Gpu_Hal_Sleep(100);

  SAMAPP_BootupConfig(phost);
  //SPI1_init(SPI_BaudRatePrescaler_4);

  LEDsSet(0x2);

  FLASH_Status FLASHStatus;
  ft_uint32_t storedMaterial = FLASH_Read_DataWord(0);
  if (storedMaterial >= 201 & storedMaterial <=203) {
	  ;
  } else {
	  FLASHStatus = FLASH_Write_DataWord(0, 201);
  }

  dloffset = API_Screen_BasicScreen(phost, SCREEN);

  // SAMAPP_API_Screen(phost, "START START START"); // 1  появляется фон и текст +

  // SAMAPP_Russian(phost);
  // dloffset = API_Screen_MainScreen(phost); // 2 появляется фон, текст background и число 777 и текст content
  // SAMAPP_API_Screen_Content(phost, dloffset, init_finished); // 2 ШРИФТ!!!

  // Info(); // 3 ничего не появляется - цикл do while
  // Sketch(); // 4 ничего не появляется - цветные полосы и while

  LEDsSet(0x1);

  Ft_Gpu_Hal_Sleep(50);

  int flag = 1;

  init_finished = 1;
  __enable_irq();


  while(1)
  {

	  LEDsSet(0x2);
	  int tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);

	  if (tag != 0){

		  SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
		  Ft_Gpu_Hal_Sleep(5);

		  switch (tag){
			case 1:
				switch (SCREEN){
					case MAIN:
						SCREEN = SETTINGS;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
					case SETTINGS:
						SCREEN = MAIN;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
					case MATERIAL:
						SCREEN = SETTINGS;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
					case LOGS:
						SCREEN = SETTINGS;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
				}
			break;
			case 2:
				switch (STATE)
			  {
				case IDLE:
				  STATE = TOUCH;
				break;
				case TOUCH:
				  A0 = A;
				  STATE = MEASURE;
				break;
				case MEASURE:
				  F1 = F;//измерение
				  A1 = A;
				  STATE = REMOVAL;
				break;
				case REMOVAL:
				  H = A;

				  T14math(&E, &ST, &SB, &HB, F1, A0, A1, H);
				  STATE = RESULTS;
				break;
				case RESULTS:
					//clear
				  STATE = TOUCH;
				break;
			  }

				SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
				Ft_Gpu_Hal_Sleep(5);
				flag = 1;
			break;
			case 3:
				STATE = IDLE;
				SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
				flag = 0;
			break;
			case 4:
				switch (SCREEN){
					case MAIN:
					break;
					case SETTINGS:
						SCREEN = MATERIAL;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
					case MATERIAL:
					break;
					case LOGS:
					break;
				}
			break;
			case 5:
				switch (SCREEN){
					case MAIN:
					break;
					case SETTINGS:
						SCREEN = LOGS;
						dloffset = API_Screen_BasicScreen(phost, SCREEN);
					break;
					case MATERIAL:
					break;
					case LOGS:
					break;
				}
			break;
			case 201:
			case 202:
			case 203:
				FLASHStatus = FLASH_Write_DataWord(0, tag);
				//ft_uint32_t storedValue = FLASH_Read_DataWord(0);
			break;
		  }

		  while (tag != 0){
			  tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
		  }

	  } else {
		  SAMAPP_API_Screen_Content(phost, SCREEN, STATE, tag, dloffset, F, A, A0, H, F1, A1, E, HB, ST, SB, init_finished);
		  tag = Ft_Gpu_Hal_Rd8(phost,REG_TOUCH_TAG);
		  Ft_Gpu_Hal_Sleep(5);
	  }


	  USB_Send_DataPair(STATE, F, A);
	  //USB_Send_State(STATE);

	  if (flag != 0)
	  	  init_finished++;

	  //SAMAPP_API_Screen_Content(phost, dloffset, init_finished);
  }
}


void EXTI0_IRQHandler()
{
    EXTI_ClearFlag(EXTI_Line0);
}


void Timer2IntrHandler (void)
{

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);

////////  float adc_value_f = get_adc_value();
////////  F = F1K * adc_value_f + F1B;

    int mean = 0;
  	for (int i = 0 ; i < DMA_BUFFER_SIZE; i++){
  		mean += DMA_BUFFER[i];
  	}
  	F = mean / DMA_BUFFER_SIZE;

  	Dacc = Dacc + F - Dout;
  	Dout = Dacc/(uint16_t)K;
  	F = Dout;
//	int sum = 0;
//	int cnt = 0;
//	for (int i =0 ; i < DMA_BUFFER_SIZE; i++){
//		if ( abs( DMA_BUFFER[i] - mean ) < 50) {
//			DMA_BUFFER_[cnt] = DMA_BUFFER[i];
//			sum += DMA_BUFFER[i];
//			cnt++;
//		}
//	}
//	sum = sum / cnt;
	//F = F1K * sum * (ADC_VOLTS / ADC_MAX_BITS) + F1B;
	//F = sum * (ADC_VOLTS / ADC_MAX_BITS);
//	F = sum;

  	F = F1K * F + F1B;
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	//DMA_Cmd(DMA1_Channel1, ENABLE);

	//костыль
//	if (F < 0.099)
//	{
//		F = 0;
//	}



  A = A1K*ReadSSI()+A1B;

  //DMA_ClearITPendingBit( DMA1_IT_TC1);
  //DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
}


void DMA1_Channel1_IRQHandler(void)
{

   DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);
   //DMA_Cmd(DMA1_Channel1, DISABLE);


}




