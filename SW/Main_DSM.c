#include "stm8l15x.h"
#include "main.h"
#include "usart_debug.h"
#include "user_typedef.h"
#include "adc.h"
#include "Main_DSM.h"
#include "configuration.h"
#include "lcd.h"
#include "system_time.h"
enum {
  SET_MAX_LEVEL=0,
  SET_MIN_LEVEL=1,
  SET_MODE=2,
}eSetupMenu;

//teArmedStatus ArmedStatus;
tStateMashine_status Main_DSM_status;
tStateMashine_status Setup_DSM_status;

extern tMainDataStruct MainDataStruct;
extern tStateMashine_status ADC_DSM_state;
extern tsSystemTime SystemTime;
static uint16_t old_max_level = 32000;
static uint16_t old_min_level = 32000;
extern uint16_t old_car_level;
//uint16_t* setup_pointer[3];


uint16_t set_key_counter;
uint16_t valve_key_counter;
tErr Main_DSM (void)
{
  switch (Main_DSM_status)
  {
    
  case TERRA_INIT:
    MainDataStruct.max_level = 800;
    MainDataStruct.min_level = 500;
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
    break;
    
case TERRA_STANDBY:
  //----------------SET_KEY-----------------
  if (SET_KEY_PRESSED() && MainDataStruct.set_key_pressed)
  {
    set_key_counter++;
    if(set_key_counter >= SET_KEY_DELAY)
    {
      MainDataStruct.set_key_pressed = 0;
      Main_DSM_status = TERRA_SETUP;
      Setup_DSM_status = ENTER_SETUP;
      set_key_counter = 0;
    };
    
  }
  else
  {
    if((set_key_counter < SET_KEY_DELAY) && (set_key_counter > PROTECT_SET_KEY_DELAY))
    {
      MainDataStruct.set_key_pressed = 0;
      if (MainDataStruct.armed == DISARMED)
      {
        MainDataStruct.armed = ARMED;
      }
      else 
      {
        MainDataStruct.armed = DISARMED;
      };
    };
    MainDataStruct.set_key_pressed = 0;
    set_key_counter = 0;
  };
  
  
  if (VALVE_KEY_PRESSED() && MainDataStruct.valve_key_pressed)
  {
    
    valve_key_counter++;
    if(valve_key_counter >= VALVE_KEY_DELAY)
    {
      MainDataStruct.valve_key_pressed = 0;
      if(MainDataStruct.valve_status == VALVE_OFF)
      {
        Main_DSM_status = TERRA_VALVE_OPEN;
        valve_key_counter = 0;
      }
      else
      {
        Main_DSM_status = TERRA_VALVE_CLOSE;
        valve_key_counter = 0;
      };
        
    };
  }
  else
  {
    valve_key_counter = 0;
  };
      //---------------------------------Battary Level Control-------------------------------------
      if(MainDataStruct.battary_level > 2700)
    {
  MainDataStruct.battary_status = MAX;
  }
  else
    {
    if ((MainDataStruct.battary_level <= 2700) && ( MainDataStruct.battary_level > 2400))
  {
  MainDataStruct.battary_status = MID;
  }
  else 
    {
    if ((MainDataStruct.battary_level <= 2400) && ( MainDataStruct.battary_level > 2000))
    {
      MainDataStruct.battary_status = LOW;
    }
    else 
      {
        if (MainDataStruct.battary_level < 2000)
        {
        MainDataStruct.battary_status = CRITICAL;
    };
    };
      };
      };
 //------------------------------------------------SENSOR_LEVEL_CONTROL-------------------
   if (MainDataStruct.car_level <= MainDataStruct.min_level)
   {
     if((MainDataStruct.armed == ARMED) && (MainDataStruct.valve_status == VALVE_OFF))
     {
       Main_DSM_status = TERRA_VALVE_OPEN;
     };
   };
   if (MainDataStruct.car_level >= MainDataStruct.max_level)
   {
     if((MainDataStruct.armed == ARMED) && (MainDataStruct.valve_status == VALVE_ON))
     {
       Main_DSM_status = TERRA_VALVE_CLOSE;
     };
   };
   //Если датчик полива не отключился в течение времени  AUTO_OFF_MANUAL_MODE, то отключить боевой режим и закрыть клапан  
   if((MainDataStruct.valve_status == VALVE_ON) && (!SystemTime.auto_off_manual_mode_timer))
   {      
       Main_DSM_status = TERRA_VALVE_CLOSE;
       MainDataStruct.armed = DISARMED; 
   };
   //=============================================================================================
//----------------------------------------------TEMPERATURE_LOCK-----------------------------------
// Не забыть что NTC резистор !!!!
   if(MainDataStruct.temperature > MainDataStruct.lock_temperature)
   {
     if (MainDataStruct.armed != LOW_TEMP)
     {
       MainDataStruct.armed = LOW_TEMP;
       Main_DSM_status = TERRA_VALVE_CLOSE;  
       old_car_level = 32000;
     };
     
   };
   
   
    



  break;
  
  case TERRA_MEASURE:
    ADC_DSM_state = GET_ADC_VALUE;
    Main_DSM_status = TERRA_STANDBY;
   
      
    break;
  case TERRA_SENSOR:
  ADC_DSM_state = GET_SENSOR_VALUE;
    Main_DSM_status = TERRA_STANDBY;  
    break;
case TERRA_SETUP:
 
  
  switch (Setup_DSM_status)
  {
  case ENTER_SETUP:
    auto_exit_reset();
    ADC_DSM_state = ADC_DEINIT; 
    lcd_clear_all();
    blink(ON);
    old_max_level = 32000;
    old_min_level = 32000;
    Setup_DSM_status = SETUP_MAX_LEVEL; 
    break;
    
  case SETUP_MAX_LEVEL:
    if(DOWN_KEY_PRESSED())
    {
      auto_exit_reset();
      for(int y=0;y<9000;y++){asm("NOP");};
      if(MainDataStruct.max_level > MainDataStruct.min_level)
        {
          MainDataStruct.max_level--;
        };
    };
    if(UP_KEY_PRESSED())
    {
      auto_exit_reset();
      for(int y=0;y<9000;y++){asm("NOP");};
      if(MainDataStruct.max_level < 999)
      {
        MainDataStruct.max_level++;
      };
    };
    
    if (old_max_level != MainDataStruct.max_level)
    {
      lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
      old_max_level = MainDataStruct.max_level;
    };
    
    if(SET_KEY_PRESSED())
    {
      auto_exit_reset();
      set_key_counter++;
      if(set_key_counter >= SET_KEY_DELAY)
      {
        MainDataStruct.set_key_pressed = 0;
        Setup_DSM_status = SETUP_MIN_LEVEL;
        set_key_counter = 0;
        lcd_clear_all();
        blink(ON);
      };
    }
    else
    {
       set_key_counter = 0;
    };
    
     if (!SystemTime.auto_exit_setup)
  {
    Setup_DSM_status = SETUP_EXIT;
  };
    break; 
    case SETUP_MIN_LEVEL:
    if(DOWN_KEY_PRESSED())
    {
      auto_exit_reset();
      for(int y=0;y<9000;y++){asm("NOP");};
      if(MainDataStruct.min_level > 0) 
        {
          MainDataStruct.min_level--;
        };
    };
    if(UP_KEY_PRESSED())
    {
      auto_exit_reset();
      for(int y=0;y<9000;y++){asm("NOP");};
      if(MainDataStruct.min_level < MainDataStruct.max_level)
      {
        MainDataStruct.min_level++;
      };
    };
    
    if (old_min_level != MainDataStruct.min_level)
    {
      lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level);
      old_min_level = MainDataStruct.min_level;
    };
   if(SET_KEY_PRESSED())
    {
      auto_exit_reset();
      set_key_counter++;
      if(set_key_counter >= SET_KEY_DELAY)
      {
        MainDataStruct.set_key_pressed = 0;
        Setup_DSM_status = SETUP_EXIT;
        set_key_counter = 0;
        //lcd_clear_all();
       // blink(OFF);
               
      };
    }
    else
    {
       set_key_counter = 0;
    };
      
     
    break; 
    
  case SETUP_EXIT:
    
    lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
    lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level);
    lcd_data_write(LCD_CAR_LEVEL,MainDataStruct.car_level);
    lcd_bat_level(MainDataStruct.battary_status);
    lcd_valve(MainDataStruct.valve_status);
    lcd_valve(MainDataStruct.armed);
    Main_DSM_status = TERRA_STANDBY;
    
    break;
    
    
    
    
  default:
    break;
  };
  
  break;

  case TERRA_VALVE_CLOSE:
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
    break;
    
    
  case TERRA_VALVE_OPEN:
   MainDataStruct.valve_status = VALVE_ON;
   SystemTime.auto_off_manual_mode_timer = AUTO_OFF_MANUAL_MODE;
   M_POWER_ON();
   VALVE_OPEN();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_OPEN();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_OPEN();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_OPEN();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
    VALVE_OPEN();
   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP");};
   VALVE_RESET();
   M_POWER_OFF();
   Main_DSM_status = TERRA_STANDBY; 
    break;
    
    
    
  default:
    break;
    
  };
  
  return OK;
};
