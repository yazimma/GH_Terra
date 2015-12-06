/*
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
*
*
*
*
*/
#include "stm8l15x.h"
#include "main.h"
#include "usart_debug.h"
#include "user_typedef.h"
#include "adc.h"
#include "Main_DSM.h"
#include "motor_DSM.h"
#include "lcd.h"
#include "system_time.h"
#include "configuration.h"
#include "stm8l15x_flash.h"
#include "math.h"
//#define __DEBUG

#define TEST_DISPLAY_DELAY 50000


tMainDataStruct MainDataStruct;
extern tStateMashine_status ADC_DSM_state;
extern tStateMashine_status Main_DSM_status;
extern tStateMashine_status Setup_DSM_status;
extern tsSystemTime SystemTime;
extern uint8_t scrool;
extern t_motor_DSM motor_DSM_state;
_Bool u = FALSE;
static uint16_t old_max_level;
static uint16_t old_min_level;
uint16_t old_car_level;
static teBattaryLevel old_battary_status;
static teValveStatus old_valve_status;
static teValveStatus old_arm_status; 
uint16_t blow_count;
uint16_t valv_count;
uint16_t bat_crit_count;
bool Go = FALSE;
_Bool yu = TRUE;
_Bool bat_critical_blink = FALSE;
//---------------------------------Прототипы функций----------------------------------------------
void clock_init (void);
void IO_init(void);
void  DSM_Init(void);
void LCD_update(void);
//-------------------------------------------------------------------------------------------------
//#pragma location = "RAM_DATA_F" 
void go_to_sleap(void)
{
  uint8_t i = 0;
 // FLASH->CR1 = 0x08;
  //while(((CLK->REGCSR)&0x80)==0x80);
/* Swith off the Regulator*/
//  CLK->REGCSR = 0x02;
//  while(((CLK->REGCSR)&0x01)==0x01);
  WFE->CR1 = 0x30;
  WFE->CR2 = 0x06;
  WFE->CR3 = 0x06;
  WFE->CR4 = 0x01;
for (i=0; i<100; i++);
  __enable_interrupt();
  wfi();
  
  //EXTI->SR1 |= 0x63;
  WFE->CR1 = 0x00;
  WFE->CR2 = 0x00;
  WFE->CR3 = 0x00;
  WFE->CR4 = 0x00;
  __enable_interrupt();
//Switch on the regulator
//  CLK->REGCSR = 0x00;
//  while(((CLK->REGCSR)&0x1) != 0x1);
}


//Прерывания клавиш -------------------------------------------------------------------------------
INTERRUPT_HANDLER(EXTI0_IRQHandler, 11)  //Кнопка SET 3
{ 
  MainDataStruct.ready_to_suspend = FALSE;
  if(!MainDataStruct.exe_mode)
{
  MainDataStruct.fast_mode = 1;
  SystemTime.off_fast_mode = FAST_MODE_DUTY;
  SystemTime.sensor_get_time = 0;
};
SystemTime.key_no_pressed_time = 0;
MainDataStruct.set_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}
INTERRUPT_HANDLER(EXTI1_IRQHandler, 10)  //Кнопка Down 2
{
  MainDataStruct.ready_to_suspend = FALSE;
  scrool = 0;
  if(!MainDataStruct.exe_mode)
{
  MainDataStruct.fast_mode = 1;
  SystemTime.off_fast_mode = FAST_MODE_DUTY;
  SystemTime.sensor_get_time = 0;
};
SystemTime.key_no_pressed_time = 0;
MainDataStruct.down_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

INTERRUPT_HANDLER(EXTI5_IRQHandler, 9) //Кнопка Up 1
{
  MainDataStruct.ready_to_suspend = FALSE;
  scrool = 0;
  if(!MainDataStruct.exe_mode)
{
  MainDataStruct.fast_mode = 1;
  SystemTime.off_fast_mode = FAST_MODE_DUTY;
  SystemTime.sensor_get_time = 0;
};
SystemTime.key_no_pressed_time = 0;
MainDataStruct.up_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}
//INTERRUPT_HANDLER(EXTI6_IRQHandler, 14) //Кнопка Valve
//{
//  MainDataStruct.ready_to_suspend = FALSE;
// if(!MainDataStruct.exe_mode)
//{
//  MainDataStruct.fast_mode = 1;
//  SystemTime.sensor_get_time = 0;
//};
// SystemTime.off_fast_mode = FAST_MODE_DUTY;
//MainDataStruct.set_key_pressed = 1;
//EXTI_ClearITPendingBit(EXTI_IT_Pin6);
//}

//-------------------------------------------------------------------------------------------------

void main(void)
{
  
  #ifdef __DEBUG
  UsartInit(9600);
  #endif //__DEBUG
  //Инициализация переферии  
  clock_init (); 
  IO_init(); 
  
  lcd_init();
  DSM_Init();
  system_timer_init();
  //------------------LOAD_FROM_EEPROM---------------------
  if ( ee_lock_temperature == 0) //Если в EEPROM ничего нет то запишем установки по умолчанию
  {
    unlock_eeprom();
    ee_lock_temperature = 18300;
    ee_zero_level = 81;
    ee_max_level = 800;
    ee_min_level = 400;
    ee_watering_protect_interval = WATERING_PROTECT_INTERVAL;
    ee_sw_lock = LOCK;
    lock_eeprom();
  };
  if(UP_KEY_PRESSED() && DOWN_KEY_PRESSED())
  {
     unlock_eeprom();
     ee_sw_lock = UNLOCK;
     
     lock_eeprom();
     for (int sw = 0; sw != SOFT_WERSION;sw++)
     {
       lcd_data_write(LCD_CAR_LEVEL,sw);
       for(uint16_t y=0;y<8000;y++){for(uint16_t w=0;w<90;w++){asm("NOP");};};        
     };
      for(uint16_t y=0;y<30000;y++){for(uint16_t w=0;w<90;w++){asm("NOP");};}; 
           
  };
  
  //Загрузим из EEPROM данные
  MainDataStruct.lock_temperature = ee_lock_temperature;
  MainDataStruct.zero_level = ee_zero_level;
  MainDataStruct.max_level = ee_max_level;
  MainDataStruct.min_level = ee_min_level;
  MainDataStruct.ready_to_suspend = FALSE;
  MainDataStruct.watering_protect_interval = ee_watering_protect_interval;
  MainDataStruct.sw_unlocked = ee_sw_lock;
  //__enable_interrupt();
   if(MainDataStruct.sw_unlocked == SOFT_STOP)
   {
      blink(ON);
      lcd_bat_level(CRITICAL);
      while(1){};
     
   };
    enableInterrupts();
  while (1)
    {
      
      
      Main_DSM ();
      ADC_DSM ();
      motor_DSM ();
      
      if(MainDataStruct.sw_unlocked != UNLOCK) //Если софт не разблокирован проверим таймер
      {
        if(SystemTime.on_timer >= LOCK_IN) //Вышло время триала?
        {
          unlock_eeprom();
          ee_sw_lock = SOFT_STOP; //Залочить 
          lock_eeprom();
          blink(ON);
          lcd_bat_level(CRITICAL);
          while(1){};
          
        };
      };

        if(Main_DSM_status != TERRA_SERVISE)
        {
          LCD_update();
        };

        if (Main_DSM_status != TERRA_STANDBY){MainDataStruct.ready_to_suspend = FALSE;};
        if (ADC_DSM_state != READY_TO_SUSPEND){MainDataStruct.ready_to_suspend = FALSE;};
        if (MainDataStruct.set_key_pressed) {MainDataStruct.ready_to_suspend = FALSE;};
        if (MainDataStruct.up_key_pressed) {MainDataStruct.ready_to_suspend = FALSE;};
        if (MainDataStruct.down_key_pressed) {MainDataStruct.ready_to_suspend = FALSE;};
        if (MainDataStruct.temporary_manual_mode) {MainDataStruct.ready_to_suspend = FALSE;};
        if (MainDataStruct.battary_level == 0) {MainDataStruct.ready_to_suspend = FALSE;};
        
if (MainDataStruct.ready_to_suspend)
{
#ifdef SLEEP_MODE_ENABLED
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    while (((CLK->SWCR)& 0x01)==0x01);
    CLK_HSICmd(DISABLE);
    __enable_interrupt();
    go_to_sleap();
#endif /*SLEEP_MODE_ENABLED*/
  CLK_HSICmd(ENABLE);
  while (((CLK->ICKCR)& 0x02)!=0x02);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  while (((CLK->SWCR)& 0x01)==0x01);
 
};
MainDataStruct.ready_to_suspend = TRUE;        
    
   
 };
  
};

void clock_init (void)
{
  
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD,ENABLE); //Включим тактирование АЦПCLK_Peripheral_LCD
  //Тактирование переферии включается непосредственно в файлах конфигурации преферии
  //CLK_HSEConfig(CLK_HSE_ON);
  //CLK_HSICmd(ENABLE);                        //Включили внутренний РЦ генератор на 16 МГц
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);     //выбрали источник тактирования системы от HSI
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);             //Включили тактирования системы
  
};

void IO_init(void)
{
  
  //LCD Ports 
  GPIO_Init(GPIOA,0xf0,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB,0xff,GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOD,0x0f,GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOE,0x3f,GPIO_Mode_Out_PP_Low_Slow); 
  //-----------------------------------------------
  //Key
  GPIO_Init(GPIOC,0x0E,GPIO_Mode_In_FL_IT);
  //Преывания по кнопкам
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);//_Low
  EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Falling);//_Low
  EXTI_SetPinSensitivity(EXTI_Pin_3,EXTI_Trigger_Falling);//_Low
 // EXTI_SetPinSensitivity(EXTI_Pin_6,EXTI_Trigger_Falling);//_Low
 
  //-----------------------------------------------
  //VALVE
  GPIO_Init(GPIOA,GPIO_Pin_2 + GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);
  //Valve Sensor
  GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC,GPIO_Pin_5,GPIO_Mode_In_FL_No_IT);
  //-----------------------------------------------
  //Internal connections
  //GPIO_Init(GPIOD,0x80,GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOC,0x64,GPIO_Mode_Out_PP_Low_Slow);
  //------------------------------------------------
  //ADC
  GPIO_Init(GPIOD,0xC0,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(GPIOF,GPIO_Pin_0,GPIO_Mode_In_FL_No_IT); 
  //------------------------------------------------
  //Timers
  GPIO_Init(GPIOD,0x30,GPIO_Mode_Out_PP_Low_Slow); 
  //-----------------------------------------------
  
};



void  DSM_Init(void)
{
  MainDataStruct.ready_to_suspend = FALSE;
  ADC_DSM_state = READY_TO_SUSPEND;
  Main_DSM_status = TERRA_INIT;
  motor_DSM_state = MOTOR_NO_INIT;
  MainDataStruct.car_level = 0;
  MainDataStruct.valve_error = FALSE;
  MainDataStruct.battary_level = 0;
   lcd_bat_level(CRITICAL);
  for(uint16_t l=0;l<TEST_DISPLAY_DELAY;l++){asm("NOP");};
  lcd_bat_level(LOW);
  for(uint16_t l=0;l<TEST_DISPLAY_DELAY;l++){asm("NOP");};
  lcd_bat_level(MID);
  for(uint16_t l=0;l<TEST_DISPLAY_DELAY;l++){asm("NOP");};
  lcd_bat_level(MAX);
  for(uint16_t l=0;l<TEST_DISPLAY_DELAY;l++){asm("NOP");};
  lcd_valve(VALVE_ON);
  for(uint16_t l=0;l<TEST_DISPLAY_DELAY;l++){asm("NOP");};
  lcd_valve(AUTO_ON);
  lcd_data_write(LCD_MAX_LEVEL,0);
  lcd_data_write(LCD_MIN_LEVEL,0);
    lcd_data_write(LCD_CAR_LEVEL,0);
    MainDataStruct.max_level = 800;
    MainDataStruct.min_level = 500;
    MainDataStruct.lock_temperature = LOCK_TEMPERATURE;
    MainDataStruct.valve_status = VALVE_OFF;
    MainDataStruct.armed = ARMED;
    MainDataStruct.temporary_manual_mode = 0;
   // MainDataStruct.valve_state = M_OPEN;
   // MainDataStruct.valve_command_done = FALSE;
    
   // MainDataStruct.valve_status = VALVE_OFF;
    MainDataStruct.max_level_lcd_on = 1;
    MainDataStruct.min_level_lcd_on = 1;
//   M_POWER_ON();
//   VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//   VALVE_RESET();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//    VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//   VALVE_RESET();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//    VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//   VALVE_RESET();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//    VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//   VALVE_RESET();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
//    VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   
   Main_DSM_status = TERRA_STANDBY; 
  
};

uint16_t level_lcd_blinc_count;

void LCD_update(void)
{ 
  
  if((Main_DSM_status == TERRA_SETUP) && ((Setup_DSM_status == SETUP_MAX_LEVEL)|(Setup_DSM_status == SETUP_TO_MANUAL)))
    {
      level_lcd_blinc_count ++;
      if(level_lcd_blinc_count >= LEVEL_LCD_BLINC_PERIOD)
      {
        level_lcd_blinc_count = 0;
        if(MainDataStruct.max_level_lcd_on == 1)
        {
          MainDataStruct.max_level_lcd_on = 0;
        }
        else
        {
          MainDataStruct.max_level_lcd_on = 1;
        };
      };
          
    }
  else 
  {
    MainDataStruct.max_level_lcd_on = 1;
  };
  //************************************************************************************
  if((Main_DSM_status == TERRA_SETUP) && ((Setup_DSM_status == SETUP_MIN_LEVEL)|(Setup_DSM_status == SETUP_TO_MANUAL))) //В режиме установки мигаем через жо.у
    {
      level_lcd_blinc_count ++;
      if(level_lcd_blinc_count >= LEVEL_LCD_BLINC_PERIOD)
      {
        level_lcd_blinc_count = 0;
        if(MainDataStruct.min_level_lcd_on == 1)
        {
          MainDataStruct.min_level_lcd_on = 0;
        }
        else
        {
          MainDataStruct.min_level_lcd_on = 1;
        };
      };
          
    }
  else 
  {
    MainDataStruct.min_level_lcd_on = 1;
  };
  
  //************************************************************************************
    
  if(MainDataStruct.max_level_lcd_on == 1)  //Если отображение разрешено то собственно показать значение
  {
    if (old_max_level != MainDataStruct.max_level) 
    {
      lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
      old_max_level = MainDataStruct.max_level;
    };
  }
  else
  {
    sim4_clr();
    sim5_clr();
    sim6_clr();
    old_max_level = 32000;
  };
  
  if(MainDataStruct.min_level_lcd_on == 1) //Если отображение разрешено то собственно показать значение
  {
    if (old_min_level != MainDataStruct.min_level)// 
    {
      lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level); //
      old_min_level = MainDataStruct.min_level;// 
    };
  }
  else
  {
    sim7_clr();
    sim8_clr();
    sim9_clr();
    old_min_level = 32000;
  };
  


  if (old_car_level != MainDataStruct.car_level)
  {
    if(MainDataStruct.armed != LOW_TEMP)
    {
      lcd_data_write(LCD_CAR_LEVEL,MainDataStruct.car_level);
      old_car_level = MainDataStruct.car_level;
    }
    else
    {
      lcd_low_temp();
    };
  };
  
  
  if (old_battary_status != MainDataStruct.battary_status)
  {
   lcd_bat_level(MainDataStruct.battary_status);
    old_battary_status = MainDataStruct.battary_status;
  };
  //Пробуем мигать краном типа ошибка клапана
 
  if( MainDataStruct.valve_error)
  {
    valv_count++;
    MainDataStruct.temporary_manual_mode = 0;
    if(valv_count > VALV_COUNT_PERIOD_BLINK)
   {
     valv_count = 0;
      
      if(u)
      {
        
        lcd_valve(ARMED);
        
      }
      else
      {
        
        lcd_valve(DISARMED);
        
      };
      u = !u;
    };
  }
  else
  {
    if (old_arm_status != MainDataStruct.armed) //Кран
  {
   lcd_valve(MainDataStruct.armed);
    old_arm_status = MainDataStruct.armed;
  };
  
    
    
  };
  
  
  
  
  //Пробуем мигать каплей
  if( MainDataStruct.temporary_manual_mode)
  {
    blow_count++;
    
    if(blow_count > BLOW_COUNT_PERIOD_BLINK)
   {
     blow_count = 0;
      yu=!yu;
      if(yu)
      {
        lcd_blow(M_CLOSE);
      }
      else
      {
        lcd_blow(M_OPEN);        
      };
    };
  }
  else
  {
    
  if (old_valve_status != MainDataStruct.valve_status)  //Капля
  {
   lcd_blow(MainDataStruct.valve_state);
    old_valve_status = MainDataStruct.valve_status;
  };
  };
  
  //Мигаем рамкой батареи при критическом разряде
  
//  if(MainDataStruct.battary_status == CRITICAL)
//  {
//    if(bat_crit_count++ >= BATTARY_CRIT_BLINK_PERIOD)
//    {
//      bat_crit_count = 0;
//      bat_critical_blink = !bat_critical_blink;
//      if(bat_critical_blink)
//        {
//          s012;
//        }
//      else
//        {
//          c012;
//        };
//    };
//  };
  
  
    
};