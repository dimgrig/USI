#include "t14-uart.h"
#include "TouchPanel.h"
#include "GLCD.h"




void ESP8266_init();

uint8_t str_compare(char * ar1, char * ar2, uint16_t length);
uint8_t ESP8266_send_AT(char * AT, uint16_t length, char * otvet, char * AT_OK, uint16_t * otvet_length);
uint8_t ESP8266_send_data(char * data, uint16_t length);

