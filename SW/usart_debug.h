#include "stm8l15x.h"
#include "user_typedef.h"


#ifndef __USART_DEBUG_H__
#define __USART_DEBUG_H__

//INTERRUPT void USART1_TX_IRQHandler(void); /* USART1 TX / TIM5 UPD/OVF/TRG/BRK */
//INTERRUPT void USART1_RX_IRQHandler(void); /* USART1 RX / TIM5 CAP */

tErr UsartInit(uint32_t boud);
tErr Usart_printString(char* str);
tErr Usart_printUINT(uint16_t simb);


#endif //__USART_DEBUG_H__