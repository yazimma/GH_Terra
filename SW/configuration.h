#ifndef __CONFIG_H__
#define __CONFIG_H__
//-========WakeUP Config===================--
//1162 = 1s
#define MAIN_PERIOD 1162
#define ADC_GET_VALUE_PERIOD 1
#define ADC_GET_SENSOR_PERIOD 5
#define AUTO_EXIT_TIMER 30
#define AUTO_OFF_MANUAL_MODE 3600

//����������� ���������� � ������������� ���������� �� ������������
//18300 = 10 C ; 12161 = 20; 22649 = 5; 28222 = 0;
#define LOCK_TEMPERATURE 18300
//---------------------------------KEYS_CONFIG_PARAM----------------------------------------------
//����� �� ������� ��������� ��� ������ �����/���� 
#define STEPS_BEFOR_FAST_STEP 10
//�������� ������� SET
#define SET_KEY_DELAY 30000
//�������� ������� VALVE
#define VALVE_KEY_DELAY 30000
//�������� �������� ������� SET
#define PROTECT_SET_KEY_DELAY 1000
#define SERVISE_MENU_DELAY 30000
//---------------------------------��������� ��������������� �������� �������������---------------
//����������� ���������� ��������������� ��������
#define HESTIREZIS 20 
//---------------------------------H-Most_MOTOR_PARAM---------------------------------------------
//����� ��������� ��������� � �������� :-)
#define DELAY_VALVE_DRIVE 30000
//=============================������_���������_�����__________________________________________
//240 ����������� ������� ����� ��-�� ���������� ���������� � ����������� �� ������ ����������. 
#define ZERO_LEVEL 240



#endif //__CONFIG_H__