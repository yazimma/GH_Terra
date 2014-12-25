#ifndef __CONFIG_H__
#define __CONFIG_H__
//-========WakeUP Config===================--
//1162 = 1s
#define MAIN_PERIOD 1162
#define ADC_GET_VALUE_PERIOD 1
#define ADC_GET_SENSOR_PERIOD 5
#define AUTO_EXIT_TIMER 30
#define AUTO_OFF_MANUAL_MODE 3600

//Температура блокировки в сопротивлении термистора на температурах
//18300 = 10 C ; 12161 = 20; 22649 = 5; 28222 = 0;
#define LOCK_TEMPERATURE 18300
//---------------------------------KEYS_CONFIG_PARAM----------------------------------------------
//Шагов до быстрой прокрутки для клавиш вверх/вниз 
#define STEPS_BEFOR_FAST_STEP 10
//Задержка клавиши SET
#define SET_KEY_DELAY 30000
//Задержка клавиши VALVE
#define VALVE_KEY_DELAY 30000
//Защитный интервал клавиши SET
#define PROTECT_SET_KEY_DELAY 1000
#define SERVISE_MENU_DELAY 30000
//---------------------------------Параметры устанавливаемых значений пользователем---------------
//Минимальный Гестерезис устанавливаемых значений
#define HESTIREZIS 20 
//---------------------------------H-Most_MOTOR_PARAM---------------------------------------------
//Время включения двигателя в попугаях :-)
#define DELAY_VALVE_DRIVE 30000
//=============================ДАТЧИК_ВЛАЖНОСТИ_ПОЧВЫ__________________________________________
//240 Минимальный перекос моста из-за неточности резисторов и супрессоров на выходе измерителя. 
#define ZERO_LEVEL 240



#endif //__CONFIG_H__