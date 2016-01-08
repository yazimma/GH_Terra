#ifndef __CONFIG_H__
#define __CONFIG_H__
//Процедура обработки данных о батарее
//BAT_LEVEL_INDICATOR_OLD - Мой вариант
//#define BAT_LEVEL_INDICATOR_OLD
//Power_save_configuration
#define SLEEP_MODE_ENABLED
//SoftWare_version
#define SOFT_WERSION 14
//LOCK       3600 = 1 h
//1728000 в секундах
#define LOCK_IN 1728000
#define TIME_OF_SIMULATE_MULFUNCTION 86400
//-========WakeUP Config===================--
//1162 = 1s
#define MAIN_PERIOD 1162
//Батарея и температура
#define ADC_GET_VALUE_PERIOD 10

#define ADC_GET_SENSOR_PERIOD_FAST_MODE 1
#define ADC_GET_SENSOR_PERIOD_EXE_MODE 1
#define EXE_MODE_DUTY 14400

#define ADC_GET_SENSOR_PERIOD_VALVE_ON 3
#define ADC_GET_SENSOR_PERIOD_VALVE_OFF 3
#define FAST_MODE_DUTY 300

#define AUTO_EXIT_TIMER 30
#define AUTO_OFF_MANUAL_MODE 3600

//Таймер возврата в режим автоматического управления после нажатия клавиши
#define TIMEOUT_FOR_RETURN_IN_ARMED_MODE 15

#define BLOW_COUNT_PERIOD_BLINK 10000
#define VALV_COUNT_PERIOD_BLINK 5000
#define LEVEL_LCD_BLINC_PERIOD 4000


//Температура блокировки в сопротивлении термистора на температурах
//18300 = 10 C ; 12161 = 20; 22649 = 5; 28222 = 0;15=15230
#define LOCK_TEMPERATURE 18300
//Период мигания данных ва режиме низкой температуры
#define INTERVAL_IN_LOW_TEMP_MODE 1
//Защитный интервал полива по умолчанию hours
#define WATERING_PROTECT_INTERVAL 1
//Время после нажатия клавиши до включения защитного интервала sec
#define NO_PRESSED_KEY_PROTECT_INTERVAL_VALID 600
//циклов работы мотора до включения индикатора ошибки клапана в рабочем порядке
#define ERROR_VALVE_PERIOD_COUNT 100000
//Циклов работы клапана до включения ошибки при стартовом тестировании
#define ERROR_VALVE_PERIOD_COUNT_START_TEST 10000000

//Период мигания в режиме сервисного меню
#define BLINK_IN_SERVISE 10000

//---------------------------------KEYS_CONFIG_PARAM----------------------------------------------
//Шагов до быстрой прокрутки для клавиш вверх/вниз 
#define STEPS_BEFOR_FAST_STEP 10
//Шагов в режиме быстрого шага
#define FAST_STEP_COUNT 20

//Задержка клавиши SET
#define SET_KEY_DELAY 40000
#define SET_KEY_DELAY_IN_MENU 5000
//Задержка клавиши VALVE
#define VALVE_KEY_DELAY 30000
//Защитный интервал клавиши SET
#define PROTECT_SET_KEY_DELAY 1000
#define SERVISE_MENU_DELAY 1500

//Длительность нажатия клавиши SET для перехода в ручной режим
#define DELAY_FOR_MANUAL_MODE 2700

//---------------------------------Параметры устанавливаемых значений пользователем---------------
//Минимальный Гестерезис устанавливаемых значений
#define HESTIREZIS 10 
//---------------------------------H-Most_MOTOR_PARAM---------------------------------------------
//Время включения двигателя в попугаях :-)
#define DELAY_VALVE_DRIVE 30000
//=============================ДАТЧИК_ВЛАЖНОСТИ_ПОЧВЫ__________________________________________
//240 Минимальный перекос моста из-за неточности резисторов и супрессоров на выходе измерителя. 
#define ZERO_LEVEL 240
//==============================Уровни заряда батареи=========================================
#ifdef BAT_LEVEL_INDICATOR_OLD

      #define BAT_MAX_LEVEL 2700
      #define BAT_MID_LEVEL 2400
      #define BAT_LOW_LEVEL 2000
      #define BAT_HESTEREZIS 100
#else
      #define BAT_MAX_LEVEL_max 4000
      #define BAT_MAX_LEVEL_min 2801

      #define BAT_MID_LEVEL_max 2800
      #define BAT_MID_LEVEL_min 2501

      #define BAT_LOW_LEVEL_max 2500
      #define BAT_LOW_LEVEL_min 2201

      #define BAT_CRITICAL_LEVEL_max 2200
      #define BAT_CRITICAL_LEVEL_min 1500
#endif /*BAT_LEVEL_INDICATOR_OLD*/

#define BATTARY_CRIT_BLINK_PERIOD 20000

#endif //__CONFIG_H__