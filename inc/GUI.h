#include "stm32l1xx.h"
#include "stm32.h"
#include "GLCD.h"
#include "TouchPanel.h"
#include "userdef.h"

  typedef enum
  { 
	ERROR_ = 0x30, /*!<  */
    IDLE = 0x31, /*!<  */
    TOUCH   = 0x32, /*!< */
    MEASURE  = 0x33, /*!<  */
    REMOVAL  = 0x34,  /*!<  */
	RESULTS  = 0x35,  /*!<  */
	RESET_  = 0x36  /*!<  */
  }State_TypeDef;
  
  typedef enum
  {
    MAIN = 0x00, /*!<  */
    SETTINGS   = 0x01, /*!< */
	MATERIAL  = 0x02, /*!<  */
    CALIBRATION  = 0x03  /*!<  */
  }Screen_TypeDef;
  

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
