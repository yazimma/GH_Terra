#include "stm8l15x.h"

#ifndef __USER_TYPEDEF_H__
#define __USER_TYPEDEF_H__
#define SET_KEY_PRESSED() !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define UP_KEY_PRESSED()  !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define DOWN_KEY_PRESSED() !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define VALVE_KEY_PRESSED() !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define M_POWER_ON()  GPIO_WriteBit(GPIOD, GPIO_Pin_7,SET);
#define M_POWER_OFF() GPIO_WriteBit(GPIOD, GPIO_Pin_7,RESET);
#define VALVE_CLOSE() {GPIO_WriteBit(GPIOE, GPIO_Pin_7,SET);\
                        GPIO_WriteBit(GPIOE, GPIO_Pin_6,RESET);}

#define VALVE_OPEN()    {GPIO_WriteBit(GPIOE, GPIO_Pin_6,SET);\
                        GPIO_WriteBit(GPIOE, GPIO_Pin_7,RESET);}

#define VALVE_RESET()   {GPIO_WriteBit(GPIOE, GPIO_Pin_6,RESET);\
                        GPIO_WriteBit(GPIOE, GPIO_Pin_7,RESET);}
#define unlock_eeprom() {FLASH_Unlock(FLASH_MemType_Data);}
#define lock_eeprom()   {FLASH_Lock(FLASH_MemType_Data);}
//--------------------------EEPROM------------------------------------------------------
#pragma location = 0x1000
__no_init uint16_t ee_lock_temperature;
#pragma location = 0x1010
__no_init uint16_t ee_zero_level;

#pragma location = 0x1020
__no_init uint16_t ee_max_level;
#pragma location = 0x1030
__no_init uint16_t ee_min_level;

//----------------------------END_OF_EEPROM---------------------------------------------

typedef enum {
READY_TO_SUSPEND,
INIT,
  READY,
  GET_ADC_VALUE,
  RE_INIT_FOR_TEMP_MEASURE,
  GET_TEMPERATURE,
  GET_SENSOR_VALUE,
  WAIT_SENSOD_DATA,
  ADC_DEINIT,
  BUSY,
  COMPLITE,
  SUSPEND,
  NO_SUSPEND,
  SUSPEND_READY,
  ADC_DONE,
  DO_IT,
  TERRA_STANDBY,
  TERRA_SETUP,
  TERRA_VALVE_OPEN,
  TERRA_VALVE_CLOSE,
  TERRA_MEASURE,
  TERRA_SENSOR,
  TERRA_INIT,
  SETUP_MAX_LEVEL,
  SETUP_MIN_LEVEL,
  ENTER_SETUP,
  SETUP_EXIT,
  TERRA_SERVISE,
  ENTER_SERVICE_MENU,
  SETUP_TEMP_LEVEL,
  SETUP_ZERO_LEVEL,
  SETUP_SERVISE_EXIT,
  SETUP_TO_MANUAL,
}tStateMashine_status;

typedef enum {
  MAX,
  MID,
  LOW,
  CRITICAL,
}teBattaryLevel;

typedef enum {
  ARMED = 0x0001,
  DISARMED = 0x0002,
  NO_INIT,
  VALVE_ON,
  VALVE_OFF,
  AUTO_ON,
  AUTO_OFF,
  ALL_OFF,
LOW_TEMP,
}teValveStatus;

typedef enum {
  LCD_MAX_LEVEL,
  LCD_MIN_LEVEL,
  LCD_CAR_LEVEL,
}teLCDData_place;


typedef enum {
OK,
ERR,
}tErr;

typedef struct {
  uint16_t max_level;
  uint16_t min_level;
  uint16_t car_level;
  teValveStatus valve_status;
  unsigned set_key_pressed:1;
  unsigned up_key_pressed:1;
  unsigned down_key_pressed:1;
  unsigned valve_key_pressed:1;
  unsigned temporary_manual_mode:1;
  unsigned max_level_lcd_on:1;
  unsigned min_level_lcd_on:1;
  unsigned fast_mode:1;
  unsigned exe_mode:1;
  teValveStatus armed;
  teBattaryLevel battary_status;
  int16_t temperature;
  uint16_t lock_temperature;
  uint16_t battary_level;
  uint16_t sensor;
  uint16_t zero_level;
  bool ready_to_suspend;
  enum {
    ADC_BUSY,
    ADC_IDLE,
    ADC_COMPLITE,
    ADC_ERR,
  }ADC_status;
 
}tMainDataStruct; 
  
typedef struct {
  uint32_t on_timer;
  uint32_t adc_get_time;
  uint32_t sensor_get_time;
  uint32_t auto_exit_setup;
  uint32_t auto_off_manual_mode_timer;
  uint32_t auto_return_in_armed_mode;
  uint32_t off_fast_mode;
  uint32_t exe_mode_off;
}tsSystemTime;

typedef enum{
  ON_TIMER,
  SUSPEND_TIMER,
}teTimers;

typedef enum {
  ON,
  OFF,
}teState_switch;
  



#endif //__USER_TYPEDEF_H__