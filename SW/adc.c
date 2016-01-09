/*
*   @@ Тут выполняется работа с АЦП в двух режимах
*       Первый режим - Непосредственных измерений получает данные о напряжении батареи и температуре
*       Второй режим - автоматических измерений работает с таймером и измеряет сопротивление датчика на переменном токе.  
*
*
*       Прерывание по оканчанию работы АЦП обрабатывает результаты измерений и сохраняет их в структуру. 
*
*
*
*
*
*/
#define Freq 1000
#define MEASURE_SAMPLES_SENSOR 5
#define CONVERSION_SAMPLES 8
#define Vref 3000
//#define Vrefint 1224

#include "stm8l15x.h"
#include "adc.h"
#include "user_typedef.h"
#include "configuration.h"
uint16_t mes_p[10];
uint16_t mes_n[10];

enum {
  BATTARY,
  TEMPERATURE,
  SENSOR,
  SENSOR_PP,
  SENSOR_NP,
} chanel;

tStateMashine_status ADC_DSM_state;
extern tMainDataStruct MainDataStruct;

uint8_t count_wrong_mesure;
static char indx;
static char ind_n;
uint32_t akkum;
uint32_t pre_battary_level;
uint32_t pre_temperature;
float pre_temp_float;
int16_t pre_sensor_p,pre_sensor_n;
uint8_t conversion_samples;
uint8_t ppm,npm;
uint32_t Vrefint;
float CalibT;
uint8_t vref_fact@0x04910;
uint8_t V90_Call@0x04911;
float calc;
bool i=FALSE;
//---------------------------------------------------------------------------------------

INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  if(i)
  {
    chanel = SENSOR_NP;
    ADC_ChannelCmd(ADC1,ADC_Channel_7, DISABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_8, ENABLE);
    ADC1->CR1 |= 0x10;  
    ADC_Cmd(ADC1,ENABLE);
    i=FALSE;
    indx=1;
    GPIO_WriteBit(GPIOD, GPIO_Pin_7, SET);
  }
  else
  {     
    chanel = SENSOR_PP;
    ADC_ChannelCmd(ADC1,ADC_Channel_8, DISABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_7, ENABLE);
    ADC1->CR1 |= 0x10;
    ADC_Cmd(ADC1,ENABLE);
    i=TRUE;
   indx=1;
  };
  ADC_SoftwareStartConv(ADC1);  
  TIM1_ClearITPendingBit(TIM1_IT_Update);
}

//----------------------------------------------------------------------------------------

INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  
  TIM1_ClearITPendingBit(TIM1_IT_CC2);
}
//-----------------------------------------------------------------------------------------
//ADC1 END OF CONVERSION INTERRUPT
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  //RTC_ITConfig(RTC_IT_WUT,DISABLE);
  
  if (indx != 0)
  {
    akkum += ADC_GetConversionValue(ADC1);
    //mes_n[ind] = ADC_GetConversionValue(ADC1);
  };
      GPIO_WriteBit(GPIOD, GPIO_Pin_7, RESET);
  indx++;
  
  if(indx >= conversion_samples+1)
  {
    indx = 0; 
    //ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);
    akkum/= conversion_samples;
    switch(chanel) 
    {
    case BATTARY:
      
      pre_battary_level = ((4096ul*Vrefint)/akkum);
      MainDataStruct.battary_level = (uint16_t)pre_battary_level;
      ADC_DSM_state = RE_INIT_FOR_TEMP_MEASURE;
      break;
      
    case TEMPERATURE:
      
        pre_temperature = akkum;
        pre_temperature*= MainDataStruct.battary_level; //
        pre_temperature /=4096;
        calc = MainDataStruct.battary_level -  pre_temperature;
        calc/=10000;
        MainDataStruct.temperature = (uint16_t)(pre_temperature / calc); 
        
        //pre_temp_float =  pre_temperature / CalibT;
//        pre_temperature/=CalibT;
        //pre_temp_float-=273;
        //pre_temp_float/=10;
     // MainDataStruct.temperature = (int16_t) pre_temperature; //pre_temp_float;
      ADC_DSM_state = ADC_DONE;
      break;
      
    case SENSOR_PP:
     
      if (ppm != MEASURE_SAMPLES_SENSOR)
      { 
        ppm++;
        pre_sensor_p+=(uint16_t)akkum;
          mes_n[ind_n++] = (uint16_t)akkum;
        akkum = 0;
       
      };
       
      break;
      
    case SENSOR_NP:
      if (npm != MEASURE_SAMPLES_SENSOR)
      { 
        npm++;
        
        pre_sensor_n+=(uint16_t)akkum;
        akkum = 0;
        };  
      
      break;
      
      
    default:
      break;
    };
    
    akkum = 0;
    
  };
  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  
}

//-------------------------------------------------------------------------------------------------


void ADC_DSM (void)
{
  MainDataStruct.ADC_status = ADC_BUSY;
  
  
  switch(ADC_DSM_state)
  {
  case READY_TO_SUSPEND: //Состояние все выключено готово ко сну
    break;
    //=================================================================  
  case GET_ADC_VALUE:  //Вход в автомат выполнения замеров АЦП
    
    chanel = BATTARY;
    conversion_samples = 4;
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, SET);
    M_POWER_ON();
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE); //Включим тактирование АЦП
    /*Можно использовать сканирование каналов,но это увеличивает потребление из-за включения ДМА. 
    хотя, возможно, потребление ДМА будет меньше чем работа ядра за время работы АЦП. Стоит провести эксперимент*/
    Vrefint = 0x00000600; //см datasheet стр 37 п5.2
    Vrefint |= vref_fact;//Взяли из памяти калибровочную переменную для опорного напряжения по адресу 0x004910
    Vrefint*=3000;
    Vrefint/=4096;
    ADC_Init(ADC1,                                         //Инит АЦП 12бит режим пределитель в еденицу преобразуем на 1МГц Однократно
             ADC_ConversionMode_Single,
             ADC_Resolution_12Bit,
             ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1,
                           ADC_Group_SlowChannels,
                           ADC_SamplingTime_4Cycles); //Сэмплирование в 4 цыкла АЦП
    
    ADC_VrefintCmd(ENABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint, ENABLE);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    __enable_interrupt();
    ADC_Cmd(ADC1,ENABLE);
    ADC_VrefintCmd(ENABLE);
    for(uint16_t y=0;y<5000;y++){asm("NOP");};
    ADC_DSM_state = DO_IT;
    break;
    //===================================================================  
    /*  //Перенастроим АЦП и используя данные полученные при замере батареи (Напряжение питания 
    это Референс для АЦП при замерах)проведем замеры на датчике температуры. Будем использовать пока внешний датчик.
    Т.к. внутренний датчик показывает какую то фигню. Возможно я его не правильно понимаю.*/
  case RE_INIT_FOR_TEMP_MEASURE:          
    chanel = TEMPERATURE;  
    //Проверяем канал температуры
//    V90_msb |= V90_Call;  //Получим откалиброванное значение из памяти.
//    CalibT = (V90_msb*3000ul)/4096; //Вычислим напряжение откалиброванное при 90 градусах референс равен 3000мв из даташита
//    CalibT/= 363;
    conversion_samples = 4;
    ADC_VrefintCmd(DISABLE);
    ADC_Cmd(ADC1,DISABLE);
    ADC_Init(ADC1,
             ADC_ConversionMode_Single,
             ADC_Resolution_12Bit,
             ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1,                          //Сэмплирование в 4 цыкла АЦП
                           ADC_Group_SlowChannels,
                           ADC_SamplingTime_4Cycles);
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, SET);
    //ADC_TempSensorCmd(ENABLE);
   // ADC_VrefintCmd(ENABLE);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint, DISABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_24, ENABLE);  //  ADC_Channel_TempSensor
    ADC_Cmd(ADC1,ENABLE);
    for(int y=0;y<30000;y++){asm("NOP");};
    
    ADC_DSM_state = DO_IT;  
    break;
    
    //Запустим АЦП
  case DO_IT:
    MainDataStruct.ADC_status = ADC_BUSY;
    ADC_SoftwareStartConv(ADC1);
    break;
    
    
    //АЦП завершило работу  
  case ADC_DONE:
    MainDataStruct.ADC_status = ADC_COMPLITE;
    ADC_DeInit(ADC1);
    //TIM1_DeInit();
    ADC_VrefintCmd(DISABLE);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, RESET);
    M_POWER_OFF();
     CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE); //Включим тактирование АЦП
    ADC_DSM_state = READY_TO_SUSPEND;
    //ADC_DSM_state = GET_SENSOR_VALUE;
    
    break;
    //-----------------------------------------------------------------------------------------    
  case GET_SENSOR_VALUE:
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE); //Включим тактирование АЦП
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,ENABLE); //Включим тактирование таймера 1
    /*Настроим по отдельности два вывхода таймера. Я не использую инвертированный выход тк он занят дисплеем
    придется дергать полярности двумя разными выходами
    */
    conversion_samples = 1;
    ppm=npm=0;
    TIM1_TimeBaseInit(1,
                      TIM1_CounterMode_CenterAligned1,
                      Freq,
                      0x00);
    
    TIM1_OC2Init(TIM1_OCMode_PWM1,
                 TIM1_OutputState_Enable,
                 TIM1_OutputNState_Disable,
                 Freq /2,
                 TIM1_OCPolarity_High,
                 TIM1_OCNPolarity_Low,
                 TIM1_OCIdleState_Reset,
                 TIM1_OCNIdleState_Reset);
    TIM1_OC3Init(TIM1_OCMode_PWM1,
                 TIM1_OutputState_Enable,
                 TIM1_OutputNState_Disable,
                 Freq /2,
                 TIM1_OCPolarity_Low,
                 TIM1_OCNPolarity_Low,
                 TIM1_OCIdleState_Set,
                 TIM1_OCNIdleState_Reset);
    TIM1_BDTRConfig(TIM1_OSSIState_Enable,
                    TIM1_LockLevel_Off,
                    0x00,
                    TIM1_BreakState_Disable,
                    TIM1_BreakPolarity_Low,
                    TIM1_AutomaticOutput_Enable);
    //TIM1_ITConfig(TIM1_IT_CC2,ENABLE);
    TIM1_ITConfig(TIM1_IT_Update,ENABLE);
    
    ADC_Init(ADC1,                                         //Инит АЦП 12бит режим пределитель в еденицу преобразуем на 1МГц Однократно
             ADC_ConversionMode_Single,
             ADC_Resolution_12Bit,
             ADC_Prescaler_1);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_SamplingTimeConfig(ADC1,
                           ADC_Group_SlowChannels,
                           ADC_SamplingTime_4Cycles); //Сэмплирование в 4 цыкла АЦП
    __enable_interrupt();
    TIM1_Cmd(ENABLE); 
    
    ADC_DSM_state = WAIT_SENSOD_DATA;
    break;
  case WAIT_SENSOD_DATA:
    if((ppm >= MEASURE_SAMPLES_SENSOR) && (npm >= MEASURE_SAMPLES_SENSOR))
    {
      ADC1->CR1 &= ~0x10;
      TIM1->CNTRH = 0x01;
      TIM1->CNTRL = 0x01;
       pre_sensor_p -= mes_n[0];
      pre_sensor_p/=ppm-1;
      pre_sensor_n/=npm;
      
      if((pre_sensor_n - pre_sensor_p) > 0)
      {
        count_wrong_mesure = 0;
        MainDataStruct.car_level = (pre_sensor_n - pre_sensor_p);//-MainDataStruct.zero_level;
        if(MainDataStruct.car_level < MainDataStruct.zero_level)
        {
           MainDataStruct.car_level = 0;
        };
      }
      else
      {
        ++count_wrong_mesure;
        if(count_wrong_mesure > 3)
        {
          count_wrong_mesure = 0;
          MainDataStruct.car_level = 0;
          
        };
        //if((pre_sensor_n - pre_sensor_p < MainDataStruct.zero_level))
      };
             
//      if (MainDataStruct.car_level > 5000)
//      {  
//        MainDataStruct.car_level = 0;
//        
//      };
//      else
//      {
//             MainDataStruct.car_level = ((uint16_t)(pre_sensor_n - pre_sensor_p))-MainDataStruct.zero_level;
//       
//      };
      ADC_DSM_state = ADC_DEINIT;
       ind_n = 0;
    };
    break;
  case ADC_DEINIT:
    indx = 0;
    akkum = 0;
    pre_battary_level = 0;
    pre_temperature = 0;
    conversion_samples = 0;
    i=FALSE;
    pre_sensor_p = 0;
    pre_sensor_n = 0;
    ppm = 0;
    npm = 0;
    MainDataStruct.ADC_status = ADC_COMPLITE;
    ADC_DeInit(ADC1);
    TIM1_DeInit();
    ADC_VrefintCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE); //Включим тактирование АЦП
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,DISABLE); //Включим тактирование таймера 1
    ADC_DSM_state = READY_TO_SUSPEND;
    break; 
    break;
    
  default:
    
    break;
    
  };
  
};
