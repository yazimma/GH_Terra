#define delay_scrool()  for(uint16_t y=0;y<8000;y++){for(uint16_t w=0;w<90;w++){asm("NOP");};}
#define delay_key_counter() for(uint16_t y=0;y<8000;y++){asm("NOP");}
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

struct {
  uint16_t pre_temp;
  uint16_t pre_zero_level;
  uint32_t pre_protect_interval;
}pre_setup;

//teArmedStatus ArmedStatus;
tStateMashine_status Main_DSM_status;
tStateMashine_status Setup_DSM_status;
tStateMashine_status Setup_servise_DSM_status;
teValveStatus save_pre_status;
extern tMainDataStruct MainDataStruct;
extern tStateMashine_status ADC_DSM_state;

extern tsSystemTime SystemTime;
static uint16_t old_max_level = 32000;
static uint16_t old_min_level = 32000;
extern uint16_t old_car_level;
extern uint16_t level_lcd_blinc_count;
uint8_t scrool;
//uint16_t* setup_pointer[3];


uint16_t set_key_counter;
uint16_t valve_key_counter;
uint16_t servise_menu_counter;
uint16_t down_key_counter;
uint16_t up_key_counter;


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
  //===================================SERVISE=====================================================
  if ((UP_KEY_PRESSED()) && (DOWN_KEY_PRESSED()))
  {
    servise_menu_counter++;
    if(servise_menu_counter > SERVISE_MENU_DELAY)
    {
      servise_menu_counter = 0;
      Main_DSM_status = TERRA_SERVISE;
      Setup_servise_DSM_status = ENTER_SERVICE_MENU; 
    };
  }
  else
  {
    servise_menu_counter = 0;
  };
  //==================================END_SERVICE===================================================
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
    MainDataStruct.set_key_pressed = 0;
    set_key_counter = 0;
  };
  //====================================VALVE_KEY_CONTROL====================================
  
//  if (VALVE_KEY_PRESSED() && MainDataStruct.valve_key_pressed)
//  {
//    
//    valve_key_counter++;
//    if(valve_key_counter >= VALVE_KEY_DELAY)
//    {
//      MainDataStruct.valve_key_pressed = 0;
//      if(MainDataStruct.valve_status == VALVE_OFF)
//      {
//        Main_DSM_status = TERRA_VALVE_OPEN;
//        valve_key_counter = 0;
//      }
//      else
//      {
//        Main_DSM_status = TERRA_VALVE_CLOSE;
//        valve_key_counter = 0;
//      };
//        
//    };
//  }
//  else
//  {
//    valve_key_counter = 0;
//  };
  
  if(UP_KEY_PRESSED() && MainDataStruct.up_key_pressed)
  {
    up_key_counter++;
    if (up_key_counter >= PROTECT_SET_KEY_DELAY)
    {
      MainDataStruct.up_key_pressed = 0;
      if(MainDataStruct.armed == DISARMED)
      {
        MainDataStruct.up_key_pressed = 0;
        Main_DSM_status = TERRA_VALVE_OPEN;
        up_key_counter = 0;
      }
      else
      {
        SystemTime.auto_return_in_armed_mode = TIMEOUT_FOR_RETURN_IN_ARMED_MODE;
        up_key_counter = 0;
        MainDataStruct.temporary_manual_mode = 1;
        Main_DSM_status = TERRA_VALVE_OPEN;
        MainDataStruct.up_key_pressed = 0;
      };
    }
  }
    else
    {
    up_key_counter = 0;
     MainDataStruct.up_key_pressed = 0;
  };
  
  if(DOWN_KEY_PRESSED() && MainDataStruct.down_key_pressed)
  {
    down_key_counter++;
    if (down_key_counter >= PROTECT_SET_KEY_DELAY)
    {
      MainDataStruct.down_key_pressed = 0;
      if(MainDataStruct.armed == DISARMED)
      {
        Main_DSM_status = TERRA_VALVE_CLOSE;
        down_key_counter = 0;
      }
      else
      {
        down_key_counter = 0;
        SystemTime.auto_return_in_armed_mode = TIMEOUT_FOR_RETURN_IN_ARMED_MODE;
        MainDataStruct.temporary_manual_mode = 1;
        Main_DSM_status = TERRA_VALVE_CLOSE;
        MainDataStruct.down_key_pressed = 0;
      };
    };
  }
    else
    {
      MainDataStruct.down_key_pressed = 0;
    down_key_counter = 0;
  };
      //---------------------------------Battary Level Control-------------------------------------
  if(MainDataStruct.battary_level >= BAT_MAX_LEVEL + BAT_HESTEREZIS)
  {
    MainDataStruct.battary_status = MAX;
  }
  else
  {
    if ((MainDataStruct.battary_level < BAT_MAX_LEVEL) && ( MainDataStruct.battary_level >= BAT_MID_LEVEL + BAT_HESTEREZIS))
    {
      MainDataStruct.battary_status = MID;
    }
    else 
    {
      if ((MainDataStruct.battary_level < BAT_MID_LEVEL) && ( MainDataStruct.battary_level >= BAT_LOW_LEVEL + BAT_HESTEREZIS))
      {
        MainDataStruct.battary_status = LOW;
      }
      else 
      {
        if (MainDataStruct.battary_level < BAT_LOW_LEVEL)
        {
          MainDataStruct.battary_status = CRITICAL;
        };
      };
    };
  };
 //------------------------------------------------SENSOR_LEVEL_CONTROL-------------------
      if(!SystemTime.auto_return_in_armed_mode)
      {
        MainDataStruct.temporary_manual_mode = 0;
      };
        
   if (MainDataStruct.car_level <= MainDataStruct.min_level)
   {
     if((MainDataStruct.armed == ARMED) && (MainDataStruct.valve_status == VALVE_OFF) && (!MainDataStruct.temporary_manual_mode))
     {
       Main_DSM_status = TERRA_VALVE_OPEN;
     };
   };
   if (MainDataStruct.car_level >= MainDataStruct.max_level)
   {
     if((MainDataStruct.armed == ARMED) && (MainDataStruct.valve_status == VALVE_ON) && (!MainDataStruct.temporary_manual_mode))
     {
       Main_DSM_status = TERRA_VALVE_CLOSE;
     };
   };
   //==============================================================================================
   //Если датчик полива не отключился в течение времени  AUTO_OFF_MANUAL_MODE, то отключить боевой режим и закрыть клапан
   //Исключено по решению заказчика при тестировании Бетта версии
//   if((MainDataStruct.valve_status == VALVE_ON) && (!SystemTime.auto_off_manual_mode_timer))
//   {      
//       Main_DSM_status = TERRA_VALVE_CLOSE;
//       MainDataStruct.armed = DISARMED; 
//   };
   //=============================================================================================
//----------------------------------------------TEMPERATURE_LOCK-----------------------------------
// Не забыть что NTC резистор !!!!
   if(MainDataStruct.temperature > MainDataStruct.lock_temperature)
   {
     if (MainDataStruct.armed != LOW_TEMP)
     {
       save_pre_status = MainDataStruct.armed; 
       MainDataStruct.armed = LOW_TEMP;
       Main_DSM_status = TERRA_VALVE_CLOSE;  
       old_car_level = 32000;
     };
   }
   else 
   {
     if (MainDataStruct.armed == LOW_TEMP)
     {
       
       MainDataStruct.armed = save_pre_status;
       old_car_level = 32000;
     };
   };
   //======================FAST_MODE_CONTROL==================================
   if(!SystemTime.off_fast_mode)
   {
     MainDataStruct.fast_mode = 0;
   };
   
  if(!SystemTime.exe_mode_off)
   {
     MainDataStruct.exe_mode = 0;
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
    case SETUP_TO_MANUAL:
      
      if (SET_KEY_PRESSED())
    {
      set_key_counter++;
      delay_key_counter();
      if(set_key_counter >= DELAY_FOR_MANUAL_MODE)
      {
        set_key_counter = 0;
        if (MainDataStruct.armed == ARMED)
        {
          MainDataStruct.armed = DISARMED;
          MainDataStruct.temporary_manual_mode = 0;
        }
        else 
        {
          MainDataStruct.armed = ARMED;
          MainDataStruct.temporary_manual_mode = 0;
        };
        Main_DSM_status = TERRA_STANDBY;
        lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
        lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level);
        lcd_data_write(LCD_CAR_LEVEL,MainDataStruct.car_level);
        lcd_bat_level(MainDataStruct.battary_status);
        lcd_valve(MainDataStruct.valve_status);
        lcd_valve(MainDataStruct.armed);
        Main_DSM_status = TERRA_STANDBY;
      };
      
    }
    else
    {
      Setup_DSM_status = SETUP_MAX_LEVEL;
       
    };
      break;
      
    case ENTER_SETUP:
      auto_exit_reset();
    ADC_DSM_state = ADC_DEINIT; 
    //lcd_clear_all();
    //blink(ON);
    //lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
    old_max_level = 32000;
    old_min_level = 32000;
    sim4_clr();   //Выключим значения макс уровня для отклика в переходе на установки макс уровня
    sim5_clr();
    sim6_clr();
    Setup_DSM_status = SETUP_TO_MANUAL;
    set_key_counter = 0;
    
   
   
    
    break;
 //==================================MAX_LEVEL_SETUP===========================================   
  case SETUP_MAX_LEVEL:
    if(DOWN_KEY_PRESSED())
    {
      auto_exit_reset();
      delay_scrool();
      level_lcd_blinc_count = 0;
      MainDataStruct.max_level_lcd_on = 1;
      //old_max_level = 32000;
      LCD_update();
      if(MainDataStruct.max_level > MainDataStruct.min_level)
        {
          scrool++;
          if(scrool < STEPS_BEFOR_FAST_STEP)
          {
            MainDataStruct.max_level--;
          }
          else
          {
            if ((( MainDataStruct.max_level-FAST_STEP_COUNT) > MainDataStruct.min_level) && (!(( MainDataStruct.max_level-FAST_STEP_COUNT) > 1000))) 
            {
              MainDataStruct.max_level-=FAST_STEP_COUNT;
            };
          };
        };
    };
//    else
//    {
//      scrool = 0;
//    };
    //***************************************************************
    if(UP_KEY_PRESSED())
    {
      auto_exit_reset();
      delay_scrool();
      level_lcd_blinc_count = 0;
      MainDataStruct.max_level_lcd_on = 1;
      //old_max_level = 32000;
      LCD_update();
      if(MainDataStruct.max_level < 999)
      {
        scrool++;
          if(scrool < STEPS_BEFOR_FAST_STEP)
          {
            MainDataStruct.max_level++;
          }
          else
          {
            if ((MainDataStruct.max_level +FAST_STEP_COUNT) < 999)
            {
              MainDataStruct.max_level+=FAST_STEP_COUNT; 
            };
          };
      };
    };
//    else
//    {
//      scrool = 0;
//    };
    
    if (old_max_level != MainDataStruct.max_level)
    {
      lcd_data_write(LCD_MAX_LEVEL,MainDataStruct.max_level);
      old_max_level = MainDataStruct.max_level;
    };
  //*********************************************************************  
    if(SET_KEY_PRESSED())
    {
      auto_exit_reset();
      set_key_counter++;
      if(set_key_counter >= SET_KEY_DELAY_IN_MENU)
      {
        MainDataStruct.set_key_pressed = 0;
        Setup_DSM_status = SETUP_MIN_LEVEL;
        set_key_counter = 0;
        //lcd_clear_all();
        //blink(ON);
        lcd_data_write(LCD_MIN_LEVEL,MainDataStruct.min_level);
        while (SET_KEY_PRESSED()){};
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
    //---------------------------------------------------------------------------------------
    //===================================MIN_LEVEL_SETUP=====================================
    case SETUP_MIN_LEVEL:
    if(DOWN_KEY_PRESSED())
    {
      auto_exit_reset();
      delay_scrool();
      level_lcd_blinc_count = 0;
      MainDataStruct.max_level_lcd_on = 1;
      MainDataStruct.min_level_lcd_on = 1;
      LCD_update();
      if(MainDataStruct.min_level > 0) 
        {
          scrool++;
          if (scrool < STEPS_BEFOR_FAST_STEP)
          {
            MainDataStruct.min_level--;
          }
          else
          {
            if ( ((MainDataStruct.min_level-FAST_STEP_COUNT) > 0) && (!((MainDataStruct.min_level-FAST_STEP_COUNT) > 1000))) //Проверим что не выходим за пределы unsigned int
            {
              MainDataStruct.min_level-=FAST_STEP_COUNT;
            };
          };
        };
    };
    
    if(UP_KEY_PRESSED())
    {
      auto_exit_reset();
     delay_scrool();
     level_lcd_blinc_count = 0;
      MainDataStruct.max_level_lcd_on = 1;
      MainDataStruct.min_level_lcd_on = 1;
      LCD_update();
      if((MainDataStruct.min_level) < MainDataStruct.max_level)
      {
        scrool++;
        if (scrool < STEPS_BEFOR_FAST_STEP)
        {
          MainDataStruct.min_level++;
        }
        else 
        {
          if((MainDataStruct.min_level + FAST_STEP_COUNT) < MainDataStruct.max_level)
          {
            MainDataStruct.min_level+=FAST_STEP_COUNT;
          };
        };
          
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
      if(set_key_counter >= SET_KEY_DELAY_IN_MENU)
      {
        MainDataStruct.set_key_pressed = 0;
        Setup_DSM_status = SETUP_EXIT;
        set_key_counter = 0;               
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
    
  case SETUP_EXIT:
    //Если нарушено правило Гестерезиса то выставить минимальный гестерезис иначе будет вечная жжужалка... 
    if ((MainDataStruct.max_level - MainDataStruct.min_level) < HESTIREZIS)  
    {
      MainDataStruct.max_level += (HESTIREZIS - (MainDataStruct.max_level - MainDataStruct.min_level));  
    };
    unlock_eeprom();
    ee_max_level = MainDataStruct.max_level;
    ee_min_level = MainDataStruct.min_level;
    lock_eeprom();
    
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
//==========================================SERVICE=============================================
  case TERRA_SERVISE:
    
    switch (Setup_servise_DSM_status)
    {
    
    case ENTER_SERVICE_MENU:
      lcd_clear_all();
      
      if ((UP_KEY_PRESSED()) && (DOWN_KEY_PRESSED()))
      {
        servise_menu_counter++;
        if(servise_menu_counter > SERVISE_MENU_DELAY)
        {
          servise_menu_counter = 0;
          lcd_data_write(LCD_CAR_LEVEL,888);
          lcd_data_write(LCD_MAX_LEVEL,888);
          blink(ON);
          while(UP_KEY_PRESSED()){};
          blink(OFF);
          if (MainDataStruct.exe_mode == 0)
          {
            MainDataStruct.exe_mode = 1;
            SystemTime.exe_mode_off = EXE_MODE_DUTY;
          }
          else
          {
            MainDataStruct.exe_mode = 0;
            SystemTime.exe_mode_off = 0;
          };
          Setup_servise_DSM_status = SETUP_SERVISE_EXIT;
           MainDataStruct.fast_mode = 0;
          pre_setup.pre_temp = MainDataStruct.lock_temperature;
          pre_setup.pre_zero_level = MainDataStruct.zero_level ;
         };
      }
      else
      {
        servise_menu_counter = 0;
        auto_exit_reset();
        pre_setup.pre_temp = MainDataStruct.lock_temperature;
        pre_setup.pre_zero_level = MainDataStruct.zero_level ;
        ADC_DSM_state = ADC_DEINIT; 
        lcd_clear_all();
        old_max_level = 32000;
        old_min_level = 32000;
        Setup_servise_DSM_status = SETUP_TEMP_LEVEL; 
      };
      break;
      //==================================MAX_LEVEL_SETUP===========================================   
    case SETUP_TEMP_LEVEL:
      
      if(DOWN_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if(pre_setup.pre_temp-100 > 0)
        {
            pre_setup.pre_temp-=100;
        };
      };
      
      //***************************************************************
      
      if(UP_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if(pre_setup.pre_temp+100 < 65000)
        {
            pre_setup.pre_temp+=100;
        };
      };
      
      
      if (old_max_level != pre_setup.pre_temp)
      {
        lcd_data_write(LCD_CAR_LEVEL,pre_setup.pre_temp/100);
        old_max_level = pre_setup.pre_temp;
      };
      //*********************************************************************  
      if(SET_KEY_PRESSED())
      {
        auto_exit_reset();
        set_key_counter++;
        if(set_key_counter >= SET_KEY_DELAY)
        {
          MainDataStruct.set_key_pressed = 0;
          old_min_level = 32000;
          Setup_servise_DSM_status = SETUP_ZERO_LEVEL;
          set_key_counter = 0;
          lcd_clear_all();
        };
      }
      else
      {
        set_key_counter = 0;
      };
      
      if (!SystemTime.auto_exit_setup)
      {
        Setup_DSM_status = SETUP_SERVISE_EXIT;
      };
      break;
      //---------------------------------------------------------------------------------------
      //===================================MIN_LEVEL_SETUP=====================================
    case SETUP_ZERO_LEVEL:
      if(DOWN_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if(pre_setup.pre_zero_level > 0) 
        {
            pre_setup.pre_zero_level--;
        };
      };
      if(UP_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if( pre_setup.pre_zero_level < 500)
        {
            pre_setup.pre_zero_level++; 
        };
      };
      
      if (old_min_level !=  pre_setup.pre_zero_level)
      {
        lcd_data_write(LCD_MAX_LEVEL, pre_setup.pre_zero_level);
        old_min_level =  pre_setup.pre_zero_level;
      };
      if(SET_KEY_PRESSED())
      {
        auto_exit_reset();
        set_key_counter++;
        if(set_key_counter >= SET_KEY_DELAY)
        {
          MainDataStruct.set_key_pressed = 0;
          Setup_servise_DSM_status = SETUP_PROT_INTERVAL;
          set_key_counter = 0;
          lcd_clear_all();
        };
      }
      else
      {
        set_key_counter = 0;
      };
      if (!SystemTime.auto_exit_setup)
      {
        Setup_servise_DSM_status = SETUP_SERVISE_EXIT;
      };  
      
      break;
      
    case SETUP_PROT_INTERVAL:
      if(DOWN_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if(pre_setup.pre_protect_interval > 0) 
        {
            pre_setup.pre_protect_interval--;
        };
      };
      if(UP_KEY_PRESSED())
      {
        auto_exit_reset();
        delay_scrool();
        if( pre_setup.pre_protect_interval < 72)
        {
            pre_setup.pre_protect_interval++; 
        };
      };
      
      if (old_min_level !=  pre_setup.pre_protect_interval)
      {
        lcd_data_write(LCD_MIN_LEVEL, pre_setup.pre_protect_interval);
        old_min_level =  pre_setup.pre_protect_interval;
      };
      if(SET_KEY_PRESSED())
      {
        auto_exit_reset();
        set_key_counter++;
        if(set_key_counter >= SET_KEY_DELAY)
        {
          MainDataStruct.set_key_pressed = 0;
          Setup_servise_DSM_status = SETUP_SERVISE_EXIT;
          set_key_counter = 0;
          lcd_clear_all();
        };
      }
      else
      {
        set_key_counter = 0;
      };
      if (!SystemTime.auto_exit_setup)
      {
        Setup_servise_DSM_status = SETUP_SERVISE_EXIT;
      };  
      
      break; 
      
    case SETUP_SERVISE_EXIT: 
      //Загрузим данные для оперативного использования
      MainDataStruct.lock_temperature = pre_setup.pre_temp;
      MainDataStruct.zero_level = pre_setup.pre_zero_level;
      MainDataStruct.watering_protect_interval = pre_setup.pre_protect_interval;
      //Загрузим данные в EEPROM
      unlock_eeprom();
      ee_lock_temperature = pre_setup.pre_temp;
      ee_zero_level = pre_setup.pre_zero_level;
      ee_watering_protect_interval = pre_setup.pre_protect_interval;
      lock_eeprom();
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
  
//=================================================================================================
  
  case TERRA_VALVE_CLOSE:
   MainDataStruct.valve_status = VALVE_OFF;
   MainDataStruct.valve_state = M_CLOSE;
//   //M_POWER_ON();
//   VALVE_CLOSE();
//   for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP"); for(uint16_t o=0;o<=20;o++){asm("NOP");};};
//   VALVE_RESET();
//   //M_POWER_OFF();
   Main_DSM_status = TERRA_STANDBY; 
   SystemTime.protect_interval = 0;
    break;
    
    
  case TERRA_VALVE_OPEN:
//    if(SystemTime.key_no_pressed_time > NO_PRESSED_KEY_PROTECT_INTERVAL_VALID)
//    {
//    if((SystemTime.protect_interval >= MainDataStruct.watering_protect_interval*3600))
//    {
//      MainDataStruct.valve_status = VALVE_ON;
//      SystemTime.auto_off_manual_mode_timer = AUTO_OFF_MANUAL_MODE;
//      MainDataStruct.valve_state = M_OPEN;
//      MainDataStruct.valve_command_done = FALSE;
////      //M_POWER_ON();
////      VALVE_OPEN();
////      for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP"); for(uint16_t o=0;o<=20;o++){asm("NOP");};};
////      VALVE_RESET();
////      //M_POWER_OFF();
//      Main_DSM_status = TERRA_STANDBY; 
//    };
//    }
//    else
//    {
      MainDataStruct.valve_status = VALVE_ON;
      SystemTime.auto_off_manual_mode_timer = AUTO_OFF_MANUAL_MODE;
      MainDataStruct.valve_state = M_OPEN;
//      //M_POWER_ON();
//      VALVE_OPEN();
//      for(uint16_t o=0;o<=DELAY_VALVE_DRIVE;o++){asm("NOP"); for(uint16_t o=0;o<=20;o++){asm("NOP");};};
//      VALVE_RESET();
//      //M_POWER_OFF();
      Main_DSM_status = TERRA_STANDBY; 
 //   };
      
    break;
    
    
    
  default:
    break;
    
  };
  
  return OK;
};
