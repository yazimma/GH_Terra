/*
*
*
*
*
*
*
*
*
*/

#include <adc_app.h>
#include "stm8l15x.h"
#include <config_mcu.h>
#include "stm8l_discovery_lcd.h"
#include <stm8l15x_it.h>
#include <user_typedef.h>



void adc_start(ADC_Channel_TypeDef ch)
{
  GPIO_WriteBit(GPIOE,GPIO_Pin_7,SET);
  //ADC_VrefintCmd(ENABLE);
  ADC_Cmd(ADC1,ENABLE);
  ADC_ChannelCmd(ADC1,ch,ENABLE);
  //ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
  ADC_SoftwareStartConv(ADC1);
 
  
   
};

void adc_off(void)
{
  ADC_Cmd(ADC1,DISABLE);
  //ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);
  //ADC_VrefintCmd(DISABLE);
 
};
