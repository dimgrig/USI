#include "t14-adc.h"

uint16_t valueVoltsMem[4] = {2048, 2048, 2048, 2048};
uint8_t initADC = 0;
         
void ADC_init()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_Mode_AIN);
        
        
  RCC_HSICmd(ENABLE);
  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);       
        
        
//Инициализация ADC
  ADC_InitTypeDef ADC_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Включаем тактирование ADC
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;  //Разрешение ADC - 12 бит
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //Выравнивание результата вправо
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  
  
  ADC_InitStruct.ADC_ScanConvMode = ENABLE; //DISABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStruct.ADC_NbrOfConversion = 10;
  
  ADC_Init(ADC1, &ADC_InitStruct);  //Конифигурируем модуль ADC заданной структурой
  /*Далее делаем следующие настройки для регулярного канала: 
  Выбор ADC - ADC1,
  канал № 5,
  число преобразований в последовательности - 1, 
  sample time - 16 тактов*/

////  ADC_Cmd(ADC1, ENABLE); //И включаем ADC
////  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET); //Ждем готовности ADC
////  ADC1->CR2 |= ADC_CR2_CONT; //Режим непрерывного преобразования.
//// ADC1->CR2 |= ADC_CR2_SWSTART;
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_24Cycles);
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

//  ADC1->CR1;
//  ADC1->CR2;
   /* Enable ADC1 */
  ADC_DMACmd(ADC1, ENABLE); //Enable ADC1 DMA
  ADC_Cmd(ADC1, ENABLE);
  /* Wait until the ADC1 is ready */

  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}

  ADC_SoftwareStartConv(ADC1);
}


float get_adc_value()
{   
    float valueVolt = 0;
      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_4Cycles);  

    ADC_SoftwareStartConv(ADC1); /*ADC1->CR2 |= ADC_CR2_SWSTART;*/
    
    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
    {

      if (initADC == 0)
      {
        initADC = 1;
        for (int k = 0; k <= 3 ; k++)
        {
          valueVoltsMem[k] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/ 
        }
      }
      else
      {
        // convert valueADC to valueVolt -> valueADC * (MAX VOLT / 2^12)
        // and also :
        // ADC_SoftwareStartConvCmdsoftwareS,
        // ADC_GetCalibrationStatus
        // with
        // return (uint16_t) ADCx->DR;
        ////valueVoltsMem[0] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/      
        
        for (int k = 0; k <= 39 ; k++)
        {
          valueVoltsMem[0] = valueVoltsMem[0] + ADC_GetConversionValue(ADC1);
          
          for (int j = 0; j <= 50; )
            j++;
        }
        
        valueVoltsMem[0] = valueVoltsMem[0]/40;
      
 
      }
      
      // фильтрация       
      for (int k = 0; k <= 3 ; k++)
      {
        valueVolt += valueVoltsMem[k];
      }
      valueVolt = valueVolt/4;

      for (int k = 3; k > 0; k--)
      {
        valueVoltsMem[k] = valueVoltsMem[k - 1];
      }


      // convert  // Volt = 3.3
      // ADC = 12 bits, so 2^12 = 4096
      valueVolt = valueVolt * (ADC_VOLTS / ADC_MAX_BITS);
      
      return valueVolt;
    }
    else
    {
      return 0;
    }
}
