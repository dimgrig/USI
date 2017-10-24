#include "GUI.h"

 uint16_t height = 320;
  uint16_t width = 240;
  uint16_t lines = 16;
  uint16_t lineHeight = 20;//height/lines;
  uint16_t padding_top = 10;
  uint16_t padding_left = 10;
  uint16_t symbol_width = 8;

    char *State_CharArray[] = {"START", "TOUCH", "MEASURE", "REMOVAL"};

  char *mainScreen[5][5] = {
    {"Current values", //Текущие показания
    "Fk= ",
    "#k= ",
    "",
    ""},

    {"Touch", //Касание
    "#0= ",
    "",
    "",
    ""},

    {"Removal", //Снятие
    "hk= ",
    "",
    "",
    ""},

    {"Measurement",
    "Fk= ",
    "#k= ",
    "",
    ""},

    {"Calculation",
    "E = ",
    "$T= ",
    "HB= ",
    "$B= "}
    };
    
  uint16_t mainScreenPoints[5][2];

  
  
  
void placeState(State_TypeDef STATE)
{
  LCD_FillRect( White, 10, 270, 110, 310);
  GUI_Text(40, 280, State_CharArray[STATE], strlen(State_CharArray[STATE]), Black, White);
}

void placeReset()
{
  LCD_FillRect( White, 130, 270, 230, 310);  
  GUI_Text(160, 280, "RESET", strlen("RESET"), Black, White);
}

void placeValue(double value_f, uint8_t i, uint8_t j, uint16_t mainScreenPoints[5][2], uint8_t digits, uint8_t reset)
{
  uint8_t value_c_length = 10;
  char value_c[value_c_length];
/*  float value_f = 1;

  for (int i = 0; i < digits; i++)
    value_f = value_f*10;
  
  uint32_t value_ui = (uint32_t)(value*value_f); 
  value_f = (float)value_ui/(uint32_t)value_f;*/
  if (reset == 0)
  {
    switch (digits)
    {
      case 0:
        sprintf(value_c, "%.0f", value_f);
      break;
      case 1:
        sprintf(value_c, "%.1f", value_f);
      break;
      case 2:
        sprintf(value_c, "%.2f", value_f);
      break;
      case 3:
        sprintf(value_c, "%.3f", value_f);
      break;
      case 4:
        sprintf(value_c, "%.4f", value_f);
      break;
      default:
        sprintf(value_c, "%.5f", value_f);
      break;
    }
  }
  else
  {
    for (int i = 0; i <10; i++)
      value_c[i] = 0x20;
  }
  
  uint16_t Color = White;
  if ((i == 0) && (j == 1))
  {   
    //костыль
    if (value_f < 10)
      GUI_Text(mainScreenPoints[i][0] + 3*symbol_width + 3*symbol_width, mainScreenPoints[i][1] + lineHeight * 1, "   ", 3, Black, Black);
    
    if ((value_f > 0) && (value_f < 30))
      Color = LCD_Green;
    else if (value_f > 30)
      Color = LCD_Red;
  }


        switch (j)
        {
          case 1:
            GUI_Text(mainScreenPoints[i][0] + 3*symbol_width, mainScreenPoints[i][1] + lineHeight * 1, value_c, (strlen(value_c)>value_c_length)?value_c_length:strlen(value_c), Color, Black); //strlen(value_c)
          break;
          case 2:
            GUI_Text(mainScreenPoints[i][0] + width/2 + 3*symbol_width, mainScreenPoints[i][1] + lineHeight * 1, value_c, (strlen(value_c)>value_c_length)?value_c_length:strlen(value_c), Color, Black);
          break; 
          case 3:
            GUI_Text(mainScreenPoints[i][0] + 3*symbol_width, mainScreenPoints[i][1] + lineHeight * 2, value_c, (strlen(value_c)>value_c_length)?value_c_length:strlen(value_c), Color, Black);
          break;
          case 4:
            GUI_Text(mainScreenPoints[i][0] + width/2 + 3*symbol_width, mainScreenPoints[i][1] + lineHeight * 2, value_c, (strlen(value_c)>value_c_length)?value_c_length:strlen(value_c), Color, Black);
          break; 
          default:
          
          break;          
        }
}

void placeMainScreen()
{
   mainScreenPoints[0][0] = padding_left;
   mainScreenPoints[0][1] = padding_top + lineHeight * 0;
   mainScreenPoints[1][0] = padding_left;
   mainScreenPoints[1][1] = padding_top + lineHeight * 3;
   mainScreenPoints[2][0] = padding_left + width/2;
   mainScreenPoints[2][1] = padding_top + lineHeight * 3;
   mainScreenPoints[3][0] = padding_left;
   mainScreenPoints[3][1] = padding_top + lineHeight * 6;
   mainScreenPoints[4][0] = padding_left;
   mainScreenPoints[4][1] = padding_top + lineHeight * 9;
  
  uint8_t N = 5;
  uint8_t sectionHeight[5];
  
  for (int i = 0; i < N; i++)
  {
    sectionHeight[i] = 1;
    for (int j = 0; j < N; j++)
    {
      
        if (strlen(mainScreen[i][j]) == 0)
          break;        

        switch (j)
        {
          case 0:
            GUI_Text(mainScreenPoints[i][0] + 0, mainScreenPoints[i][1]  + 0, mainScreen[i][j], strlen(mainScreen[i][j]), White, Black);
          break;
          case 1:
            GUI_Text(mainScreenPoints[i][0] + 0, mainScreenPoints[i][1] + lineHeight * 1, mainScreen[i][j], strlen(mainScreen[i][j]), White, Black);
            sectionHeight[i] = 2;
          break;
          case 2:
            GUI_Text(mainScreenPoints[i][0] + width/2, mainScreenPoints[i][1] + lineHeight * 1, mainScreen[i][j], strlen(mainScreen[i][j]), White, Black);
          break; 
          case 3:
            GUI_Text(mainScreenPoints[i][0] + 0, mainScreenPoints[i][1] + lineHeight * 2, mainScreen[i][j], strlen(mainScreen[i][j]), White, Black);
            sectionHeight[i] = 3;
          break;
          case 4:
            GUI_Text(mainScreenPoints[i][0] + width/2, mainScreenPoints[i][1] + lineHeight * 2, mainScreen[i][j], strlen(mainScreen[i][j]), White, Black);
          break; 
        }
    }
  }
  
  uint8_t margin = padding_left/2;
  for (int i = 0; i < N; i++)
  {
    LCD_DrawLine( mainScreenPoints[i][0] - margin, mainScreenPoints[i][1] - margin, width + margin - padding_left, mainScreenPoints[i][1] - margin, LCD_Red);
    LCD_DrawLine( width + margin - padding_left, mainScreenPoints[i][1] - margin, width + margin - padding_left, mainScreenPoints[i][1] + (sectionHeight[i] - 0)*lineHeight + margin, LCD_Red);
    LCD_DrawLine( width + margin - padding_left, mainScreenPoints[i][1] + (sectionHeight[i] - 0)*lineHeight + margin, mainScreenPoints[i][0] - margin, mainScreenPoints[i][1] + (sectionHeight[i] - 0)*lineHeight + margin, LCD_Red);
    LCD_DrawLine( mainScreenPoints[i][0] - margin, mainScreenPoints[i][1] + (sectionHeight[i] - 0)*lineHeight + margin, mainScreenPoints[i][0] - margin, mainScreenPoints[i][1] - margin, LCD_Red);
  }  
  

}

void placeValuesReset()
{
                  placeValue(0, 1, 1, mainScreenPoints, 3, 1);//A0
                  placeValue(0, 2, 1, mainScreenPoints, 3, 1);//H
                  
                  placeValue(0, 3, 1, mainScreenPoints, 1, 1);//F
                  placeValue(0, 3, 2, mainScreenPoints, 3, 1);//A1
                  
                  placeValue(0, 4, 1, mainScreenPoints, 0, 1); //E
                  placeValue(0, 4, 2, mainScreenPoints, 2, 1);//ST  //digits must be 0!?   
                  placeValue(0, 4, 3, mainScreenPoints, 2, 1);//HB //digits must be 0!?
                  placeValue(0, 4, 4, mainScreenPoints, 2, 1);//SB //digits must be 0!?
}

void placeValuesTouch(double x)
{
  placeValue(x, 1, 1, mainScreenPoints, 3, 0);
}

void placeValuesRemoval(double x)
{
  placeValue(x, 2, 1, mainScreenPoints, 3, 0);
}

void placeValuesMeasurement(double x, double y)
{
  placeValue(x, 3, 1, mainScreenPoints, 1, 0);
  placeValue(y, 3, 2, mainScreenPoints, 3, 0);
}

void placeValuesMath(double x, double y, double z, double w)
{
  placeValue(x, 4, 1, mainScreenPoints, 0, 0); 
  placeValue(y, 4, 2, mainScreenPoints, 2, 0); //digits must be 0!?   
  placeValue(z, 4, 3, mainScreenPoints, 2, 0); //digits must be 0!?
  placeValue(w, 4, 4, mainScreenPoints, 2, 0); //digits must be 0!?
}

void placeValuesADC(double x, double y)
{
  placeValue(x, 0, 1, mainScreenPoints, 1, 0);
  placeValue(y, 0, 2, mainScreenPoints, 3, 0);
}
