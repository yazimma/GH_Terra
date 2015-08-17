#ifndef __CONFIG_H__
#define __CONFIG_H__
//Power_save_configuration
//#define SLEEP_MODE_ENABLED
//SoftWare_version
#define SOFT_WERSION 14
//LOCK       3600 = 1 h
//1728000
#define LOCK_IN 1728000
//-========WakeUP Config===================--
//1162 = 1s
#define MAIN_PERIOD 1162
//������� � �����������
#define ADC_GET_VALUE_PERIOD 10

#define ADC_GET_SENSOR_PERIOD_FAST_MODE 1
#define ADC_GET_SENSOR_PERIOD_EXE_MODE 1
#define EXE_MODE_DUTY 14400

#define ADC_GET_SENSOR_PERIOD_VALVE_ON 3
#define ADC_GET_SENSOR_PERIOD_VALVE_OFF 3
#define FAST_MODE_DUTY 300

#define AUTO_EXIT_TIMER 30
#define AUTO_OFF_MANUAL_MODE 3600
#define TIMEOUT_FOR_RETURN_IN_ARMED_MODE 30
#define BLOW_COUNT_PERIOD_BLINK 10000
#define VALV_COUNT_PERIOD_BLINK 5000
#define LEVEL_LCD_BLINC_PERIOD 4000


//����������� ���������� � ������������� ���������� �� ������������
//18300 = 10 C ; 12161 = 20; 22649 = 5; 28222 = 0;
#define LOCK_TEMPERATURE 18300
//�������� �������� ������ �� ��������� hours
#define WATERING_PROTECT_INTERVAL 1
//����� ����� ������� ������� �� ��������� ��������� ��������� sec
#define NO_PRESSED_KEY_PROTECT_INTERVAL_VALID 600
//������ ������ ������ �� ��������� ���������� ������ �������
#define ERROR_VALVE_PERIOD_COUNT 100000
//---------------------------------KEYS_CONFIG_PARAM----------------------------------------------
//����� �� ������� ��������� ��� ������ �����/���� 
#define STEPS_BEFOR_FAST_STEP 10
//�������� ������� SET
#define SET_KEY_DELAY 40000
#define SET_KEY_DELAY_IN_MENU 5000
//�������� ������� VALVE
#define VALVE_KEY_DELAY 30000
//�������� �������� ������� SET
#define PROTECT_SET_KEY_DELAY 1000
#define SERVISE_MENU_DELAY 30000
#define DELAY_FOR_MANUAL_MODE 1500
//---------------------------------��������� ��������������� �������� �������������---------------
//����������� ���������� ��������������� ��������
#define HESTIREZIS 10 
//---------------------------------H-Most_MOTOR_PARAM---------------------------------------------
//����� ��������� ��������� � �������� :-)
#define DELAY_VALVE_DRIVE 30000
//=============================������_���������_�����__________________________________________
//240 ����������� ������� ����� ��-�� ���������� ���������� � ����������� �� ������ ����������. 
#define ZERO_LEVEL 240
//==============================������ ������ �������=========================================
#define BAT_MAX_LEVEL 2700
#define BAT_MID_LEVEL 2400
#define BAT_LOW_LEVEL 2000
#define BATTARY_CRIT_BLINK_PERIOD 20000
#define BAT_HESTEREZIS 100

#endif //__CONFIG_H__