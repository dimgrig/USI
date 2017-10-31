#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32.h"

#define ADC_VOLTS 3.3
#define ADC_MAX_BITS 4095
#define F1K 0.6f
#define F1B -31.2f

#define ADC_ExternalTrigConv_None   ((uint32_t)0x000E0000)

void ADC_init();
float get_adc_value();

