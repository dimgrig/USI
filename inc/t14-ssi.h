#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32.h"

#define A1K 0.00002861
//0.000057220
#define A1B 0

void InitSSI(void);
void SSI_CLK_HIGH();
void SSI_CLK_LOW();
uint8_t SSI_DATA_PORT();
uint32_t ReadSSI(void);
