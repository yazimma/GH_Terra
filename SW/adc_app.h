/*
*
*
*
*
*
*
*
*
*
*
*
*/
#include "stm8l15x.h"

#ifndef __ADC_APP__
#define __ADC_APP__

void adc_start(ADC_Channel_TypeDef ch);
void adc_stop(void);
void adc_off(void); 
#endif // __ADC_APP__