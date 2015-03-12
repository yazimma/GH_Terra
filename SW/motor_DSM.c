#include "stm8l15x.h"
#include "configuration.h"
#include "main.h"
#include "usart_debug.h"
#include "user_typedef.h"
#include "adc.h"
#include "Main_DSM.h"
#include "lcd.h"
#include "system_time.h"

t_motor_DSM motor_DSM_state;
extern tMainDataStruct MainDataStruct;
uint32_t error_valve_count;
void motor_DSM (void)
{
switch(motor_DSM_state)
{
  case MOTOR_NO_INIT:

motor_DSM_state = M_CLOSE;


  break;

case M_READY:
  
  if (MainDataStruct.valve_command_done == FALSE)
  {
    if(MainDataStruct.valve_state == M_CLOSE)
    {
      motor_DSM_state = M_CLOSE;
    };
    if(MainDataStruct.valve_state == M_OPEN)
    {
      motor_DSM_state = M_OPEN;
    };
  };
    
  
    
    
break;

case M_CLOSE: 
  
  VALVE_SENSOR_ON();
  for(uint16_t y=0;y<8000;y++){for(uint16_t w=0;w<90;w++){asm("NOP");};};
  VALVE_CLOSE();
  motor_DSM_state = GO_TO_CLOSE;
  
  
break;

  
  
case M_OPEN:
  VALVE_SENSOR_ON();
  for(uint16_t y=0;y<8000;y++){for(uint16_t w=0;w<90;w++){asm("NOP");};};
  VALVE_CLOSE();
  motor_DSM_state = GO_TO_OPEN; 
  
  



break;
  
case GO_TO_CLOSE:
  MainDataStruct.ready_to_suspend = FALSE;
  if (error_valve_count >= ERROR_VALVE_PERIOD_COUNT)
  {
    MainDataStruct.valve_error = TRUE;
     VALVE_RESET();
  }
  else
  {
    error_valve_count++;
  };
  if(VALVE_SENSOR_CLOSE()) 
  {
    error_valve_count = 0;
    VALVE_RESET();
    motor_DSM_state = M_READY;
    MainDataStruct.valve_command_done = TRUE;
  };
  break;
    
  case GO_TO_OPEN:
    MainDataStruct.ready_to_suspend = FALSE;
     if (error_valve_count >= ERROR_VALVE_PERIOD_COUNT)
  {
    MainDataStruct.valve_error = TRUE;
     VALVE_RESET();
  }
  else
  {
    error_valve_count++;
  };
   if(VALVE_SENSOR_OPEN()) 
  {
    error_valve_count = 0;
    VALVE_RESET();
    motor_DSM_state = M_READY;
    MainDataStruct.valve_command_done = TRUE;
  }; 
  break;
  
default:
  
  break;
      
  };
    
    
    
    
    
  };
    