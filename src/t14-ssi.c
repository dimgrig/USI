#include "t14-ssi.h"


void InitSSI(void)
{
  GPIO_InitTypeDef  CLK_SSI_init;  // создаем структуру

  CLK_SSI_init.GPIO_Mode = GPIO_Mode_OUT;  // направление - выход
  CLK_SSI_init.GPIO_OType = GPIO_OType_PP;  // ƒвухтактный выход
  CLK_SSI_init.GPIO_PuPd = GPIO_PuPd_UP;  // 
  CLK_SSI_init.GPIO_Speed = GPIO_Speed_40MHz;  
  CLK_SSI_init.GPIO_Pin = GPIO_Pin_9; 

  GPIO_Init(GPIOC, &CLK_SSI_init);  // ‘ункци€, выполн€юща€ настройку портов
  
  GPIO_InitTypeDef  DATA_SSI_init;  // создаем структуру

  DATA_SSI_init.GPIO_Mode = GPIO_Mode_IN;  // направление - выход
  DATA_SSI_init.GPIO_Speed = GPIO_Speed_40MHz;  
  DATA_SSI_init.GPIO_PuPd = GPIO_PuPd_DOWN;  // 
  DATA_SSI_init.GPIO_Pin = GPIO_Pin_8; 

  GPIO_Init(GPIOC, &DATA_SSI_init);  // ‘ункци€, выполн€юща€ настройку портов
}

void SSI_CLK_HIGH()
{
  GPIO_SetBits(GPIOC, GPIO_Pin_9);
}

void SSI_CLK_LOW()
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_9);
}

uint8_t SSI_DATA_PORT()
{
  return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
}

uint32_t ReadSSI(void)  
{ 
    uint8_t  bit_count; 
    uint32_t result = 0; 
    uint32_t  portdata; 
    uint16_t N = 0;
    uint16_t M = 2;
    
    SSI_CLK_HIGH();
    
    for (int i = 0; i < M; )
      i++;  
    
    SSI_CLK_LOW();
    
    for (int i = 0; i < M; )
      i++;  
    
    SSI_CLK_HIGH();
    
    for (int i = 0; i < M; )
      i++;  
    
    SSI_CLK_LOW();
    
    uint16_t j = 0;
    
    for (int i = 0; i < 100; )
    {
      i++; 
      if (SSI_DATA_PORT()==0)
      {
        break;
      }
    }
    
    SSI_CLK_HIGH();
         
    for (int i = 0; i < M; )
      i++;
        
    for (bit_count=0; bit_count<19; bit_count++) 
    { 
        // falling edge on clock port 
        //SSI_CLK_PORT &= ~(1 << SSI_CLK_BIT);         
        SSI_CLK_LOW();
        
        for (int i = 0; i < N; )
          i++;
      
        // left-shift the current result 
        result = (result << 1);    
 
        // read the port data  
        portdata = SSI_DATA_PORT(); 
        
        for (int i = 0; i < N; )
          i++;
        
        result = result | portdata;
        // rising edge on clock port, data changes 
        //SSI_CLK_PORT |= (1 << SSI_CLK_BIT);  
        SSI_CLK_HIGH();
 
        // evaluate the port data (port set or clear) 
/*        if ( (portdata & (1 << SSI_DATA_PORT())) != 0) 
        { 
            // bit is set, set LSB of result 
            result = result | 0x01;   
        }  // if */
        
        for (int i = 0; i < N; )
          i++;
    } // for 
    return result; 
} 