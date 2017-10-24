#include "stm32l1xx.h"
#include "stm32.h"
#include "GLCD.h"
#include "TouchPanel.h"
#include "userdef.h"

  typedef enum
  { 
    IDLE = 0x00, /*!<  */
    TOUCH   = 0x01, /*!< */
    MEASURE  = 0x02, /*!<  */
    REMOVAL  = 0x03  /*!<  */
  }State_TypeDef;
  

  
void placeState(State_TypeDef STATE);
void placeReset();
void placeValue(double value_f, uint8_t i, uint8_t j, uint16_t mainScreenPoints[5][2], uint8_t digits, uint8_t reset);
void placeMainScreen();

void placeValuesReset();
void placeValuesTouch(double x);
void placeValuesRemoval(double x);
void placeValuesMeasurement(double x, double y);
void placeValuesMath(double x, double y, double z, double w);
void placeValuesADC(double x, double y);
