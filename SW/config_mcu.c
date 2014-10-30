/*
*   Файл конфигурации для проекта Green_house_terra
*
*
*
*
*
*
*
*
*

*
*
*/

#include "stm8l15x.h"
#include <stm8l15x_conf.h>

#include <config_mcu.h>

void clock_config(void)
{

 CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSE);
 CLK_LSEConfig(CLK_LSE_ON);
 while(CLK_GetFlagStatus(CLK_FLAG_LSERDY)==RESET); 
 CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
 
  
};

void LCD_config(void)
{
  
  
  
  
};



void config_ports(void)

{
  
  
  
  
};









