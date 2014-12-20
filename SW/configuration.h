#ifndef __CONFIG_H__
#define __CONFIG_H__
//-========WakeUP Config===================--
//1162 = 1s
#define MAIN_PERIOD 1162
#define ADC_GET_VALUE_PERIOD 1
#define ADC_GET_SENSOR_PERIOD 5
#define AUTO_EXIT_TIMER 30
#define AUTO_OFF_MANUAL_MODE 3600
//1822 = 10 C ; 1563 = 20; 1945 = 5; 2062 = 0;
#define LOCK_TEMPERATURE 1822


#define SET_KEY_DELAY 30000
#define VALVE_KEY_DELAY 30000
#define DELAY_VALVE_DRIVE 30000
#define PROTECT_SET_KEY_DELAY 1000


#endif //__CONFIG_H__