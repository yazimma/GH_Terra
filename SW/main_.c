/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    28-June-2013
  * @brief   Main program body
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */  
	
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include <config_mcu.h>
#include "stm8l_discovery_lcd.h"
#include <stm8l15x_it.h>
#include <user_typedef.h>
#include <stdio.h>
#include <delay.h>
/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    main_struct_t main_structure;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
//char ad_value_in_string[7]={"terra"};
void main(void)
{
  
  
  clock_config();
  tim4_config();
  tim2_on();
  tim3_on();
  adc_init();
 LCD_GLASS_Init();
 config_ports();
 enableInterrupts();
 
  //LCD_GLASS_DisplayString((uint8_t*)ad_value_in_string);
  /* Infinite loop */
  while (1)
  {
//    //main_structure.indx++;
//    
//    for(uint16_t c=20000;c!=0;c--){};
//    asm("NOP");
//     // disableInterrupts();
//      sprintf(main_structure.adc_value_string,"%u",main_structure.adc_calc2);
//      LCD_GLASS_DisplayString((uint8_t*)main_structure.adc_value_string);
//      main_structure.indx = 0;
     // enableInterrupts();
   // };  
    
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
