#include "stm8l15x.h"
#include "user_typedef.h"
#include "configuration.h"


tsSystemTime SystemTime;

extern tStateMashine_status Main_DSM_status;

INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
//  GPIO_WriteBit(GPIOF, GPIO_Pin_0, SET);
//  for(uint16_t l=0;l<10ul;l++){asm("NOP");};
//   GPIO_WriteBit(GPIOF, GPIO_Pin_0, RESET);
  SystemTime.on_timer++; //Таймер на 136 лет .... Гы!
  if (SystemTime.sensor_get_time > 0)
  {
    SystemTime.sensor_get_time--;
  };
  if(SystemTime.adc_get_time > 0)
  {
    SystemTime.adc_get_time--;
  };
  if (SystemTime.auto_exit_setup > 0)
{
  SystemTime.auto_exit_setup--;
};

  if(Main_DSM_status != TERRA_SETUP)
  {
    if(!SystemTime.adc_get_time)
{
   SystemTime.adc_get_time = ADC_GET_VALUE_PERIOD;
   Main_DSM_status = TERRA_MEASURE;
};
if(!SystemTime.sensor_get_time)
{
   SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD;
   Main_DSM_status = TERRA_SENSOR;
};

if(SystemTime.auto_off_manual_mode_timer > 0)
{
   SystemTime.auto_off_manual_mode_timer --;
};



  };
  RTC_ClearITPendingBit(RTC_IT_WUT);
}

void auto_exit_reset(void)
{
  SystemTime.auto_exit_setup = AUTO_EXIT_TIMER;
};


void system_timer_init(void)
{ 
  RTC_WakeUpCmd(DISABLE);
 RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);  //2.4.8.16
 
 RTC_SetWakeUpCounter(MAIN_PERIOD);
 RTC_ITConfig(RTC_IT_WUT,ENABLE);
 RTC_WakeUpCmd(ENABLE);
 SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD ;
  SystemTime.adc_get_time = 1;  
 
 
 
 };


uint16_t get_time(teTimers counter)
{
  
  
  return 0;
};