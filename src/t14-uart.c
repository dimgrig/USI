#include "t14-uart.h"


// USART Receiver buffer
#define RX_BUFFER_SIZE 350
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_wr_index=0,rx_rd_index=0;
volatile uint16_t rx_counter=0;
volatile uint8_t rx_buffer_overflow=0;

// USART Transmitter buffer
#define TX_BUFFER_SIZE 350
volatile uint8_t  tx_buffer[TX_BUFFER_SIZE];
volatile uint16_t tx_wr_index=0,tx_rd_index=0;
volatile uint16_t tx_counter=0;


void USART_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef USART_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);   
 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2); //PD5 to TX USART2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); //PD6 to RX USART2 

  // PD5 -> TX UART.
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  //PD6  -> RX UART. 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);        
  
  //USART
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;               
  USART_Init(USART2, &USART_InitStructure);             

  /* NVIC configuration */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);    
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
}


void USART2_IRQHandler(void)
{ 
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
    if ((USART2->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) == 0)
    {                       
      rx_buffer[rx_wr_index++]=(uint8_t)(USART_ReceiveData(USART2)& 0xFF);
      if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
      if (++rx_counter == RX_BUFFER_SIZE)
      {
        rx_counter=0;
        rx_buffer_overflow=1;
      }
    }
    else USART_ReceiveData(USART2);//вообще здесь нужен обработчик ошибок, а мы просто пропускаем битый байт
  }

/*  if(USART_GetITStatus(USART2, USART_IT_ORE_RX) == SET) //прерывание по переполнению буфера
  {
     USART_ReceiveData(USART2); //в идеале пишем здесь обработчик переполнения буфера, но мы просто сбрасываем этот флаг прерывания чтением из регистра данных.
  }*/

  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  {   
    if (tx_counter)
    {
      --tx_counter;
      USART_SendData(USART2,tx_buffer[tx_rd_index++]);
      if (tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
    }
    else
    {
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);                  
    }
  }
}

uint8_t get_char(void)
{
  uint8_t data;
//  while (rx_counter==0)
//    ;
  if (rx_counter!=0)
  {
    data=rx_buffer[rx_rd_index++];
      
    if (rx_rd_index == RX_BUFFER_SIZE) 
      rx_rd_index=0;
    
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    --rx_counter;
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    return data;
  }
  else
    return 0x00;
}

void put_char(uint8_t c)
{
  while (tx_counter == TX_BUFFER_SIZE);
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    
  if (tx_counter || (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET))
  {
     tx_buffer[tx_wr_index++]=c;
     if (tx_wr_index == TX_BUFFER_SIZE) 
       tx_wr_index=0;
     
     ++tx_counter;
     USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  }
  else
     USART_SendData(USART2,c);
}

void put_str(unsigned char *s, uint16_t length)
{
  uint16_t i;
  for (i = 0; i<length; i++)
    put_char(*s++);
}

void get_str(char *array_out, uint16_t *length)
{
  //char array[RX_BUFFER_SIZE];
  uint16_t i = 0;
  
  while (rx_counter!=0)
  {
  //for(i = 0; i < sizeof(AT_OK); i++)
    *array_out= (char)get_char();
    array_out++;
    i++;
  }
  
  //strncpy(str, array, i);
  //array_out = &array;
  *length = i;
}


void put_int(int32_t data)
{
  unsigned char temp[10],count=0;
  if (data<0) 
  {
    data=-data;
    put_char('-');
  }
                
  if (data)
  {
    while (data)
    {
      temp[count++]=data%10+'0';
      data/=10;                       
    }               
                    
    while (count)   
    {
      put_char(temp[--count]);                
    }
  }
  else put_char('0');     
        
}

void UART_rx_buffer_clear()
{
  rx_wr_index=0;
  rx_counter=0;
}

/*UART echo*/
/*  
    data = get_char();
    if(data != -1) //Проверяем поступление данных от компьютера
    {
      put_char(data); 
    } 
*/

