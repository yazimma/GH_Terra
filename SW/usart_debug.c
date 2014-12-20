#include "stm8l15x.h"
#include "main.h"
#include "string.h"
#include "usart_debug.h"
#include "stdlib.h"
#include "stm8l15x_clk.h"
#include "stdio.h"

tErr UsartCommandDo(char* command);
tErr Usart_printUINT(uint16_t simb);
char usart_tx_bufer[32];
char bufer[7];
char usart_rx_byte;


uint8_t intterupt_saver;
uint8_t indx;
extern tMainDataStruct MainDataStruct;
//-------------USART INT-------------------------------------------------------
INTERRUPT_HANDLER(USART1_TX_IRQHandler, 27)
{
  
   USART_SendData8(USART1,usart_tx_bufer[indx]);
   
   if(indx > sizeof(usart_tx_bufer)) 
   {
     USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
     indx = 0;
   };
   
   if(usart_tx_bufer[indx] == 0x00)
   {
    USART_ITConfig(USART1,USART_IT_TXE,DISABLE); indx = 0;
   }
   else
   {
      indx++;
   };
  
     
   USART_ClearFlag(USART1,USART_FLAG_TC);
}

INTERRUPT_HANDLER(USART1_RX_IRQHandler, 28)
{
  usart_rx_byte = USART_ReceiveData8(USART1);
  UsartCommandDo(&usart_rx_byte);
}



tErr UsartInit(uint32_t boud)
{
   CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
   USART_Init (USART1,boud,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,0x0c);// | USART_Mode_Tx = 0x0c
   USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
   return OK;
};

tErr Usart_printString(char* str)
{
  while(USART1->CR2 & 0x80){};
  memcpy(&usart_tx_bufer,str,32);
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
  __enable_interrupt();
  return OK;
};

tErr Usart_printUINT(uint16_t simb)
{
  while(USART1->CR2 & 0x80){};
  sprintf(bufer,"%u",simb);
  memcpy(&usart_tx_bufer,bufer,32);
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
  __enable_interrupt();
  return OK;
};


tErr UsartCommandDo(char* command)
{
  switch(*command)
  {
  case 'q':
  
    MainDataStruct.max_level++;
    
    break;
    
  case 'a':
    
    MainDataStruct.max_level--;
    
    break;
  case 'w':
    
    MainDataStruct.car_level++;
    
    break;
   case 's':
    
    MainDataStruct.car_level--;
    
    break; 
    
    
  default:
    break;
  };
  
  
  
  return OK;
};