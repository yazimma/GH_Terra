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
#include "lcd.h"
#include "system_time.h"
#include "configuration.h"
#include "math.h"
//#define __DEBUG

#define TEST_DISPLAY_DELAY 50000


tMainDataStruct MainDataStruct;
extern tStateMashine_status ADC_DSM_state;
extern tStateMashine_status Main_DSM_status;
extern tStateMashine_status Setup_DSM_status;
extern tsSystemTime SystemTime;
extern uint8_t scrool;


static uint16_t old_max_level;
static uint16_t old_min_level;
uint16_t old_car_level;
static teBattaryLevel old_battary_status;
static teValveStatus old_valve_status;
static teValveStatus old_arm_status; 
bool Go = FALSE;
//---------------------------------Прототипы функций----------------------------------------------
void clock_init (void);
void IO_init(void);
void  DSM_Init(void);
void LCD_update(void);
//-------------------------------------------------------------------------------------------------
//Прерывания клавиш -------------------------------------------------------------------------------
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)  //Кнопка SET
{
MainDataStruct.valve_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin0);
}
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)  //Кнопка Down
{
  scrool = 0;
//  if((Main_DSM_status == TERRA_SETUP) && (Setup_DSM_status == SETUP_MAX_LEVEL))
//  {
//    if(MainDataStruct.max_level > MainDataStruct.min_level)
//    {
//      MainDataStruct.max_level--;
//    };
//  };
//  if((Main_DSM_status == TERRA_SETUP) && (Setup_DSM_status == SETUP_MIN_LEVEL))
//  {
//    if(MainDataStruct.min_level > 20)
//    {
//      MainDataStruct.min_level--;
//    };
//  };
MainDataStruct.down_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

INTERRUPT_HANDLER(EXTI5_IRQHandler, 13) //Кнопка Up
{
  scrool = 0;
//  if((Main_DSM_status == TERRA_SETUP) && (Setup_DSM_status == SETUP_MAX_LEVEL))
//  {
//    if(MainDataStruct.max_level < 999)
//    {
//      MainDataStruct.max_level++;
//    };
//  };
//  if((Main_DSM_status == TERRA_SETUP) && (Setup_DSM_status == SETUP_MIN_LEVEL))
//  {
//    if(MainDataStruct.min_level < MainDataStruct.max_level)
//    {
//      MainDataStruct.min_level++;
//    };
//  };
MainDataStruct.up_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin5);
}
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14) //Кнопка Valve
{

MainDataStruct.set_key_pressed = 1;
EXTI_ClearITPendingBit(EXTI_IT_Pin6);
}

//-------------------------------------------------------------------------------------------------

void main(void)
{
  
  #ifdef __DEBUG
  UsartInit(9600);
  #endif //__DEBUG
  //Инициализация переферии  
  IO_init(); 
  clock_init (); 
  lcd_init();
  DSM_Init();
  system_timer_init();
  //------------------LOAD_FROM_EEPROM---------------------
  if ( ee_lock_temperature == 0) //Если в EEPROM ничего нет то запишем установки по умолчанию
  {
    unlock_eeprom();
    ee_lock_temperature = 18300;
    ee_zero_level = 250;
    ee_max_level = 800;
    ee_min_level = 400;
    lock_eeprom();
  };
  //Загрузим из EEPROM данные
  MainDataStruct.lock_temperature = ee_lock_temperature;
  MainDataStruct.zero_level = ee_zero_level;
  MainDataStruct.max_level = ee_max_level;
  MainDataStruct.min_level = ee_min_level;
  
  while (1)
    {
      //while (!Go){asm("NOP");};
      
      Main_DSM ();
      ADC_DSM ();
      if(Main_DSM_status != TERRA_SETUP)  //Если в режиме установки не обновлять экран. 
      {
        if(Main_DSM_status != TERRA_SERVISE)
        {
          LCD_update();
        };
      };
      
      
      
      
#ifdef __DEBUG
    
    Usart_printString("MAX_L: "); 
    Usart_printUINT(MainDataStruct.max_level);
    Usart_printString("| MIN_L: "); 
    Usart_printUINT(MainDataStruct.min_level);
    Usart_printString("| CAR_L: "); 
    Usart_printUINT(MainDataStruct.car_level);
    //====================================================================
    Usart_printString("| KEYS-> |"); 
    if(MainDataStruct.set_key_pressed)
    {
      Usart_printString("-@-|");
    }
    else
    {
      Usart_printString("-O-|");
    };
    if(MainDataStruct.up_key_pressed)
    {
      Usart_printString("-@-|");
    }
    else
    {
      Usart_printString("-O-|");
    };
    if(MainDataStruct.down_key_pressed)
    {
      Usart_printString("-@-|");
    }
    else
    {
      Usart_printString("-O-|");
    };
    if(MainDataStruct.valve_key_pressed)
    {
      Usart_printString("-@-|");
    }
    else
    {
      Usart_printString("-O-|");
    };
    //==========================================================================
    Usart_printString(" Temp: ");
    Usart_printUINT(MainDataStruct.temperature);
    Usart_printString("| BAT: ");
    Usart_printUINT(MainDataStruct.battary_level);
    Usart_printString("V |");
    Usart_printString(" Valve status:");
    if(MainDataStruct.valve_status)
    {
      Usart_printString(" OPEN ");
    }
    else
    {
      Usart_printString(" CLOSE ");
    };
    Usart_printString("\r\n");
#endif //__DEBUG
    
    
    };
  
};

void clock_init (void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE); //Включим тактирование АЦП
  
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD,ENABLE); //Включим тактирование АЦПCLK_Peripheral_LCD
  //Тактирование переферии включается непосредственно в файлах конфигурации преферии
  CLK_HSEConfig(CLK_HSE_ON);
  //CLK_HSICmd(ENABLE);                        //Включили внутренний РЦ генератор на 16 МГц
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);     //выбрали источник тактирования системы от HSI
  CLK_SYSCLKSourceSwitchCmd(ENABLE);             //Включили тактирования системы
  
};

void IO_init(void)
{
  
  //LCD Ports 
  GPIO_Init(GPIOA,0xf0,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOB,0xff,GPIO_Mode_Out_PP_Low_Fast); 
  GPIO_Init(GPIOD,0x0f,GPIO_Mode_Out_PP_Low_Fast); 
  GPIO_Init(GPIOE,0x3f,GPIO_Mode_Out_PP_Low_Fast); 
  //-----------------------------------------------
  //Key
  GPIO_Init(GPIOC,0x63,GPIO_Mode_In_FL_IT);
  //Преывания по кнопкам
  EXTI_SetPinSensitivity(EXTI_Pin_0,EXTI_Trigger_Falling);//_Low
  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);//_Low
  EXTI_SetPinSensitivity(EXTI_Pin_5,EXTI_Trigger_Falling);//_Low
  EXTI_SetPinSensitivity(EXTI_Pin_6,EXTI_Trigger_Falling);//_Low
  enableInterrupts();
  //-----------------------------------------------
  //VALVE
  GPIO_Init(GPIOE,0xC0,GPIO_Mode_Out_PP_Low_Slow);
  //-----------------------------------------------
  //Internal connections
  GPIO_Init(GPIOD,0x80,GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOF,0x01,GPIO_Mode_Out_PP_Low_Slow);
  //------------------------------------------------
  //ADC
  GPIO_Init(GPIOD,0x40,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(GPIOC,0x90,GPIO_Mode_In_FL_No_IT); 
  //------------------------------------------------
  //Timers
  GPIO_Init(GPIOD,0x30,GPIO_Mode_Out_PP_Low_Fast); 
  //-----------------------------------------------
  
};



void  DSM_Init(void)
{
  ADC_DSM_state = READY_TO_SUSPEND;
  Main_DSM_status = TERRA_INIT;
  MainDataStruct.car_level = 0;
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
    MainDataStruct.armed = DISARMED;
    MainDataStruct.valve_status = VALVE_OFF;
   M_POWER_ON();
   VALVE_CLOSE();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_CLOSE();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_CLOSE();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_CLOSE();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_CLOSE();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   M_POWER_OFF();
   Main_DSM_status = TERRA_STANDBY; 
  
};

void LCD_update(void)
{
  if (old_max_level != MainDataStruct.max_level) 
  {
    lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
    old_max_level = MainDataStruct.max_level;
  };
  
  if (old_min_level != MainDataStruct.min_level)// 
  {
    lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level); //
    old_min_level = MainDataStruct.min_level;// 
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
  if (old_valve_status != MainDataStruct.valve_status)
  {
   lcd_valve(MainDataStruct.valve_status);
    old_valve_status = MainDataStruct.valve_status;
  };
  if (old_arm_status != MainDataStruct.armed)
  {
   lcd_valve(MainDataStruct.armed);
    old_arm_status = MainDataStruct.armed;
  };
  
  
    
};