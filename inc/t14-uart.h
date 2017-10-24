#include <stdint.h>
#include "stm32l1xx.h"
#include "stm32.h"


//definition
void USART_init(void);
uint8_t get_char(void);
void put_char(uint8_t);
void put_str(unsigned char *s, uint16_t length);
void get_str(char *str, uint16_t *length);
void put_int(int32_t data);
void UART_rx_buffer_clear();