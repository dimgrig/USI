#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32.h"

#define ADC_VOLTS 3.3
#define ADC_MAX_BITS 4095
#define F1K 150
#define F1B -3

#define ADC_ExternalTrigConv_None   ((uint32_t)0x000E0000)
#define ADC_SampleTime ADC_SampleTime_96Cycles

void ADC_init();
float get_adc_value();

