#include "stm8l15x.h"
#include "user_typedef.h"
#include "configuration.h"
#include "lcd.h"

tsSystemTime SystemTime;
extern tMainDataStruct MainDataStruct;
extern _Bool in_low_temp_indicator;
extern tStateMashine_status Main_DSM_status;
extern uint16_t interval_in_low_temp_mode;
extern uint16_t old_car_level;

INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
//  GPIO_WriteBit(GPIOF, GPIO_Pin_0, SET);
//  for(uint16_t l=0;l<10ul;l++){asm("NOP");};
//  GPIO_WriteBit(GPIOF, GPIO_Pin_0, RESET);
  SystemTime.on_timer++; //Таймер на 136 лет .... Гы!
  
  //Вычитатель времени получения данных от датчика влажности
  
  if (SystemTime.sensor_get_time > 0)
  {
    SystemTime.sensor_get_time--;
  };
  
  //Вычитатель времени получения данных  о батареи и температуре.
  
  if(SystemTime.adc_get_time > 0)
  {
    SystemTime.adc_get_time--;
  };
  //Вычитатель времени автоматического выхода из меню
  if (SystemTime.auto_exit_setup > 0)
{
  SystemTime.auto_exit_setup--;
};



if(Main_DSM_status != TERRA_SETUP) //Если не находимся в режиме установки
{
  if (Main_DSM_status != TERRA_SERVISE) //И если не находимся в сервисном меню
  {
    if(!SystemTime.adc_get_time) //Если пришло время измерить температуру и батарею
    {
      SystemTime.adc_get_time = ADC_GET_VALUE_PERIOD;
      Main_DSM_status = TERRA_MEASURE; //Установили состояние автомата
    };
    if(!SystemTime.sensor_get_time)//Если пришло время для измерения влажности
    {
      if(!MainDataStruct.exe_mode) //Перенастройка времени измерения влажности для режима выставка
      {
        if(!MainDataStruct.fast_mode)//Режим быстрых измерений после нажатия на кнопку. 
        {
          if (MainDataStruct.valve_status == VALVE_ON)
          {
            SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD_VALVE_ON;
          }
          else
          {
            SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD_VALVE_OFF; 
          };
          Main_DSM_status = TERRA_SENSOR;
        }
        else
        {
          SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD_FAST_MODE;
          Main_DSM_status = TERRA_SENSOR;
        };
      }
      else
      {
        SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD_EXE_MODE;
        Main_DSM_status = TERRA_SENSOR;
      };
    };
    
    if(SystemTime.off_fast_mode > 0)
    {
       SystemTime.off_fast_mode--;
    };
    
    if(SystemTime.auto_off_manual_mode_timer > 0)
    {
      SystemTime.auto_off_manual_mode_timer --;
    };
  };
};

if(SystemTime.auto_return_in_armed_mode > 0)
{
  SystemTime.auto_return_in_armed_mode--;
};
if(SystemTime.exe_mode_off > 0)
{
  SystemTime.exe_mode_off--;
};

if( SystemTime.sensor_get_time == SystemTime.adc_get_time)   //Чтобы не совпадали опросы АЦП
{
  SystemTime.sensor_get_time+=2;
  SystemTime.adc_get_time+=4;
};
 //Индикатор в режиме низкой температуры
 if (MainDataStruct.armed != LOW_TEMP)
  {
     asm("NOP");
  }
  else
  {
    
    if (in_low_temp_indicator)
    {
      if(interval_in_low_temp_mode ==0) //Чтобы не мерцало сообщение Low
      {
        lcd_low_temp();
      };
      if(interval_in_low_temp_mode > INTERVAL_IN_LOW_TEMP_MODE)
      {
        in_low_temp_indicator = !in_low_temp_indicator;
        old_car_level = 32000;
        interval_in_low_temp_mode = 0;
      }
      else
      {
        interval_in_low_temp_mode++;
      };
    }
    else
    {
      if (old_car_level != MainDataStruct.car_level)
      {
        lcd_data_write(LCD_CAR_LEVEL,MainDataStruct.car_level);
        old_car_level = MainDataStruct.car_level;
      };
      if(interval_in_low_temp_mode > INTERVAL_IN_LOW_TEMP_MODE)
      {
        in_low_temp_indicator = !in_low_temp_indicator;
        interval_in_low_temp_mode = 0;
      }
      else
      {
        interval_in_low_temp_mode++;
      }; 
    };
    
  };
//END_OF_Индикатор в режиме низкой температуры

//Счетчик времени от последней нажатой клавиши
SystemTime.key_no_pressed_time++;
//счетчик времени от последнего полива
SystemTime.protect_interval++;

MainDataStruct.ready_to_suspend = FALSE;
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
 SystemTime.sensor_get_time = ADC_GET_SENSOR_PERIOD_VALVE_ON ;
  SystemTime.adc_get_time = 1;  
 
 
 
 };


uint16_t get_time(teTimers counter)
{
  
  
  return 0;
};