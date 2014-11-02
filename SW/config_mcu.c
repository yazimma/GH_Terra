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
 CLK_HSICmd(ENABLE);
 CLK_RTCClockConfig(CLK_RTCCLKSource_HSI,CLK_RTCCLKDiv_64);
 CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
 CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
 while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)==RESET); 
 CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
 CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); 
 CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
 CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
 CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
 CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
};

void tim4_config(void)
{
   TIM4_TimeBaseInit(TIM4_Prescaler_1024,0xFF);
   TIM4_ITConfig(TIM4_IT_Update,ENABLE);
   TIM4_Cmd(DISABLE);
};

 void tim2_on(void)
{
   TIM2_TimeBaseInit(TIM2_Prescaler_2,TIM2_CounterMode_Up,0x0800);
   TIM2_OC1Init(TIM2_OCMode_PWM2,TIM2_OutputState_Enable,0x0400,TIM2_OCPolarity_High,TIM2_OCIdleState_Set);
   TIM2_OC2Init(TIM2_OCMode_PWM2,TIM2_OutputState_Enable,0x0400,TIM2_OCPolarity_Low ,TIM2_OCIdleState_Set); 
   TIM2_CCxCmd(TIM2_Channel_1,ENABLE);
   TIM2_CCxCmd(TIM2_Channel_2,ENABLE);
   TIM2_BKRConfig(TIM2_OSSIState_Enable,TIM2_LockLevel_Off,TIM2_BreakState_Disable,TIM2_BreakPolarity_High,TIM2_AutomaticOutput_Enable);
   TIM2_ITConfig(TIM2_IT_CC1,ENABLE);
   TIM2_ITConfig(TIM2_IT_CC2,DISABLE);
   TIM2_Cmd(ENABLE);
};

void tim3_on(void)
{
   TIM3_TimeBaseInit(TIM3_Prescaler_32,TIM3_CounterMode_Up,0xffff);
   TIM3_ITConfig(TIM3_IT_CC1,ENABLE);
   TIM3_Cmd(ENABLE);
};

void adc_init(void)
{
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);
  ADC_VrefintCmd(ENABLE);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_4Cycles);
  ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
  ADC_Cmd(ADC1,ENABLE);
};



void config_ports(void)
{
  GPIO_Init(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOE,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Fast);
  
  
  
};









