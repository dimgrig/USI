#include "ESP8266.h"

 
  char AT[4] = {'A', 'T', '\r', '\n'};   //AT<CR><LF>
  char AT_OK[11] = {'A', 'T', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};  //"AT"+'\r'+'\r'+'\n'+'\r' //+"OK"+'\r'+'\n' //  \r\r\n\r\nOK\r\n" 
  char * pAT = AT;
  char * pAT_OK = AT_OK;

  char ATE1[6] = {'A', 'T', 'E', '1', '\r', '\n'};  //ATE1<CR><LF>
  char ATE1_OK[13] = {'A', 'T', 'E', '1', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};
  char * pATE1 = ATE1;
  char * pATE1_OK = ATE1_OK;
  
  char AT_CWMODE[13] = {'A', 'T', '+', 'C', 'W', 'M', 'O', 'D', 'E', '=', '2', '\r', '\n'};  //AT+CWMODE=2<CR><LF>
  char AT_CWMODE_OK[20] = {'A', 'T', '+', 'C', 'W', 'M', 'O', 'D', 'E', '=', '2', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};
  char * pAT_CWMODE = AT_CWMODE;
  char * pAT_CWMODE_OK = AT_CWMODE_OK;
  
  char AT_CWDHCP_DEF[19] = {'A', 'T', '+', 'C', 'W', 'D', 'H', 'C', 'P', '_', 'D', 'E', 'F', '=', '0', ',', '1', '\r', '\n'};  //AT+CWDHCP_DEF=0,1<CR><LF>
  char AT_CWDHCP_DEF_OK[26] = {'A', 'T', '+', 'C', 'W', 'D', 'H', 'C', 'P', '_', 'D', 'E', 'F', '=', '0', ',', '1', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};
  char * pAT_CWDHCP_DEF = AT_CWDHCP_DEF;
  char * pAT_CWDHCP_DEF_OK = AT_CWDHCP_DEF_OK;   
    
  char AT_CIPMUX[13] = {'A', 'T', '+', 'C', 'I', 'P', 'M', 'U', 'X', '=', '1', '\r', '\n'};  //AT+CIPMUX=1<CR><LF>
  char AT_CIPMUX_OK[20] = {'A', 'T', '+', 'C', 'I', 'P', 'M', 'U', 'X', '=', '1', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};
  char * pAT_CIPMUX = AT_CIPMUX;
  char * pAT_CIPMUX_OK = AT_CIPMUX_OK;
  
  char AT_CIPSERVER[21] = {'A', 'T', '+', 'C', 'I', 'P', 'S', 'E', 'R', 'V', 'E', 'R', '=', '1', ',', '8', '0', '8', '0', '\r', '\n'};  //AT+CIPSERVER=1,8080<CR><LF>
  char AT_CIPSERVER_OK[28] = {'A', 'T', '+', 'C', 'I', 'P', 'S', 'E', 'R', 'V', 'E', 'R', '=', '1', ',', '8', '0', '8', '0', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n'};
  char * pAT_CIPSERVER = AT_CIPSERVER;
  char * pAT_CIPSERVER_OK = AT_CIPSERVER_OK;   
    
  char AT_CIPSEND[16] = {'A', 'T', '+', 'C', 'I', 'P', 'S', 'E', 'N', 'D', '=', '0', ',', '8', '\r', '\n'};  //AT+CIPSEND=0,8<CR><LF>
  //char AT_CIPSEND[17] = {'A', 'T', '+', 'C', 'I', 'P', 'S', 'E', 'N', 'D', '=', '0', ',', '4', '3', '\r', '\n'};  //AT+CIPSEND=0,8<CR><LF>
  char AT_CIPSEND_OK[24] = {'A', 'T', '+', 'C', 'I', 'P', 'S', 'E', 'N', 'D', '=', '0', ',', '8', '\r', '\r', '\n', '\r', '\n', 'O', 'K', '\r', '\n', '>'};
  char * pAT_CIPSEND = AT_CIPSEND;
  char * pAT_CIPSEND_OK = AT_CIPSEND_OK;  
  
  
  char AT_SEND[10] = {'H', 'E', 'L', 'L', 'o', 'E', 'S', 'P', '\r', '\n'};  //TEXT<CR><LF>
  //char AT_SEND_[43] = {'T', 'R', 'E', 'T', 'Y', 'A', 'K', 'O', 'V', ' ', 'V', '.', 'N', '.', ' ', 'R', 'E', 'C', 'E', 'I', 'V', 'E', ' ', 'M', 'E', 'S', 'S', 'A', 'G', 'E', ' ', 'B', 'Y', ' ', 'W', 'I', '-', 'F', 'I', '!', '\n', '\r', '\n'};  //TEXT<CR><LF>
  
  //<CR><CR><LF>busy s...<CR><LF><CR><LF>Recv 8 bytes<CR><LF><CR><LF>SEND OK<CR><LF><NUL>

  char AT_SEND_OK[42] = {'\r', '\r', '\n', 'b', 'u', 's', 'y', ' ', 's', '.', '.', '.', '\r', '\n', '\r', '\n', 'R', 'e', 'c', 'v', ' ', '8', ' ',
                         'b', 'y', 't', 'e', 's', '\r', '\n', '\r', '\n', 'S', 'E', 'N', 'D', ' ', 'O', 'K', '\r', '\n', '\0'};
  char * pAT_SEND = AT_SEND;
  char * pAT_SEND_OK = AT_SEND_OK;  
  
  
  /*****************************************************************************/
  
  

uint8_t str_compare(char * ar1, char * ar2, uint16_t length)
{
  uint8_t result = 1;
  uint8_t i = 0;
  
  if (length == 0)
  {
    result = 0;
  }
  else
  {
    for (i = 0; i < length; i++)
    {
      if (*ar1 == *ar2)
      {
        ar1++;
        ar2++;
      }
      else
      {
        result = 0;
        break;
      }
    }
  }
  
  return result;
}


void ESP8266_init()
{
  char otvet[350];
  uint16_t otvet_length = 0;
  uint8_t result = 0;
  
  //AT
  result = ESP8266_send_AT(pAT, sizeof(AT), &otvet[0], pAT_OK, &otvet_length);
     
  GUI_Text(5, 10, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 10, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 10, "XX", strlen("OK"), White, Black);   
  }
  
  //ATE1
  result = ESP8266_send_AT(pATE1, sizeof(ATE1), &otvet[0], pATE1_OK, &otvet_length);
     
  GUI_Text(5, 30, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 30, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 30, "XX", strlen("OK"), White, Black);   
  }
  
  //AT_CWMODE
  result = ESP8266_send_AT(pAT_CWMODE, sizeof(AT_CWMODE), &otvet[0], pAT_CWMODE_OK, &otvet_length);
     
  GUI_Text(5, 50, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 50, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 50, "XX", strlen("OK"), White, Black);   
  }
  
  //AT_CWDHCP_DEF
  result = ESP8266_send_AT(pAT_CWDHCP_DEF, sizeof(AT_CWDHCP_DEF), &otvet[0], pAT_CWDHCP_DEF_OK, &otvet_length);
     
  GUI_Text(5, 70, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 70, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 70, "XX", strlen("OK"), White, Black);   
  }  
  
  //AT_CIPMUX
  result = ESP8266_send_AT(pAT_CIPMUX, sizeof(AT_CIPMUX), &otvet[0], pAT_CIPMUX_OK, &otvet_length);
     
  GUI_Text(5, 90, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 90, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 90, "XX", strlen("OK"), White, Black);   
  }
  
  //AT_CIPSERVER
  result = ESP8266_send_AT(pAT_CIPSERVER, sizeof(AT_CIPSERVER), &otvet[0], pAT_CIPSERVER_OK, &otvet_length);
     
  GUI_Text(5, 110, &otvet[0], otvet_length, White, Black);
  if (result == 1)
  {
    GUI_Text(5, 110, "OK", strlen("OK"), White, Black);   
  }
  else
  {
    GUI_Text(5, 110, "XX", strlen("OK"), White, Black);   
  }
 
}

uint8_t ESP8266_send_AT(char * AT, uint16_t length, char * otvet, char * AT_OK, uint16_t * otvet_length)
{
  //  while(USART_GetITStatus(USART2, USART_IT_RXNE) != SET) /////////Изменить!!!!!!!!!!!!!
//  {;}
//  for(i = 0; i < 2000; i++)
//    xxx++;  
  //while (rx_counter!=0)
  //for(i = 0; i < sizeof(AT_OK); i++)
  //  otvet[i]= get_char();

  /*  
  put_str(pATE1, sizeof(ATE1)); 
  DelayUS(100000);
  get_str(&otvet[0], &otvet_length);
  
  
  if (otvet_length != 0)
  {
    aaa = (uint8_t)otvet_length;
    //GUI_Text(5, 145, &aaa, sizeof((uint8_t)otvet_length), White, Black);   
    GUI_Text(5, 125, &otvet[0], otvet_length, White, Black);
    if (str_compare( &otvet[0], pATE1_OK, otvet_length) == 1)
    {
      GUI_Text(200, 145, "OK", strlen("OK"), White, Black);   
    }
  }
  else
  {
    GUI_Text(5, 145, "NO ATE1_OK answer", strlen("NO ATE1_OK answer"), White, Black); 
  }
  */
  
  uint8_t result = 0; 

  put_str(AT, length); 
  DelayUS(100000);
  get_str(otvet, otvet_length);
  
  if (otvet_length != 0)
  {
    if (str_compare( otvet, AT_OK, *otvet_length) == 1)
    {
      result = 1;  
    }
  }  
  
  return result;
}




uint8_t ESP8266_send_data(char * data, uint16_t length)
{
    char otvet[350];
    uint16_t otvet_length = 0;
    uint8_t result = 0;
    
    //AT_CIPSEND
    result = ESP8266_send_AT(pAT_CIPSEND, sizeof(AT_CIPSEND), &otvet[0], pAT_CIPSEND_OK, &otvet_length);
     
    GUI_Text(5, 170, &otvet[0], otvet_length, White, Black);
    if (result == 1)
    {
      GUI_Text(5, 170, "OK", strlen("OK"), White, Black);   
    }
    else
    {
      GUI_Text(5, 170, "XX", strlen("XX"), White, Black); 
    }
    
    //AT_SEND
    result = ESP8266_send_AT(pAT_SEND, sizeof(AT_SEND), &otvet[0], pAT_SEND_OK, &otvet_length);
     
    GUI_Text(5, 230, &otvet[0], otvet_length, White, Black);
    if (result == 1)
    {
      GUI_Text(5, 230, "OK", strlen("OK"), White, Black);   
    }
    else
    {
      GUI_Text(5, 230, "XX", strlen("XX"), White, Black); 
    }
    
    return result;
}