 #include "stm8l15x.h"

#ifndef __USER_TYPEDEF__
#define __USER_TYPEDEF__

typedef struct{
  uint8_t indx;
  uint8_t indx2;
  uint16_t adc_calc1;
  uint16_t adc_calc2;
  uint16_t adc_value1;
  uint16_t adc_value2;
  char adc_value_string[7];
enum{
ADC_BUSY,
ADC_COMPLIT,
ADC_CH1,
ADC_CH2,
}adc_state;

  
  
  
  
}main_struct_t;




#endif // __USER_TYPEDEF__