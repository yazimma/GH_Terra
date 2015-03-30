#include "stm8l15x.h"
#include "lcd.h"
#include "user_typedef.h"
#include <string.h>
#include <stdio.h>

char lcd_bufer[4];
char lcd_string[4];
extern tMainDataStruct MainDataStruct; 

void lcd_low_temp(void)
{
 sim1_clr();
sim2_clr();
sim3_clr();
#ifdef __dash
s200;
s202;
s204;
#else
s300;
s100;
s001;
s102;
s202;
s103;
s003;
#endif //__dash
  
};

void lcd_init(void)
{
 LCD_Init(LCD_Prescaler_2, 
LCD_Divider_16,
LCD_Duty_1_4,
LCD_Bias_1_3,
LCD_VoltageSource_Internal);
LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xff);
LCD_PortMaskConfig(LCD_PortMaskRegister_1,0xff);
LCD_PortMaskConfig(LCD_PortMaskRegister_2,0x03);
LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);
LCD_Cmd(ENABLE);
LCD_ContrastConfig(LCD_Contrast_Level_5);

};

void lcd_bat_level(teBattaryLevel level)
{
  LCD->RAM[0]&=~0x55;
  switch (level)
  {
  case MAX:
    LCD->RAM[0] |= 0x55;
    LCD->CR1 &= ~0xe0;
    break;
  case MID:
    LCD->RAM[0] |= 0x54;
    LCD->CR1 &= ~0xe0;
    break;
  case LOW:
    LCD->RAM[0] |= 0x50;
    LCD->CR1 &= ~0xe0;
    break;
  case CRITICAL:
    
    LCD->RAM[0] |= 0x40;
    LCD->CR1 |= 0xe0;
    break;
    
  default:
    
    break;
  }
};
void lcd_blow(t_motor_DSM sts)
{
  switch(sts)
  {
      case M_OPEN:
  LCD->RAM[1]|=(1<<6);//LCD->RAM[1]|= (1<<4); 
    break;
    
  case M_CLOSE:
   LCD->RAM[1]&=(~(1<<6)); //LCD->RAM[1]&=(~(1<<4));  
    break;
      
  };
  
  
};
void lcd_valve(teValveStatus valve_status)
{  //LCD->RAM[1]&= ~0x50;
  switch (valve_status)
  {
     
  
    
  case ARMED:
    LCD->RAM[1]&=(~(1<<4));//LCD->RAM[1]|= (1<<4);//LCD->RAM[1]|=(1<<6);
    break;
    
  case DISARMED:
    if(!MainDataStruct.temporary_manual_mode)
    {
      LCD->RAM[1]|= (1<<4);//LCD->RAM[1]&=(~(1<<4));//LCD->RAM[1]&=(~(1<<6)); 
    };
    break;
  case ALL_OFF:
      LCD->RAM[1]&= ~0x50;
    break;
  default:
     LCD->RAM[1]&= ~0x50;
    break;
  };
};
void lcd_clear_all (void)
{
  for (int y=0;y<13;y++){  LCD->RAM[y] &=0x00; };
};

void blink(teState_switch st)
{
  if(st == ON)
  {
    LCD->CR1 |= 0xE0;
  }
  else
  {
    LCD->CR1 &= ~0xE0;
  };
};

void lcd_digit_clear(void)
{
  LCD->RAM[0] &=~0xAA;//357);
  LCD->RAM[1] &= ~0xAF;//(~(1<<4))|(~(1<<6));//    46
  for (int y=2;y<13;y++){  LCD->RAM[y] &=0x00; };
};
  
void lcd_data_write(teLCDData_place place,uint16_t data)
{
  for (uint8_t o=0;o!= sizeof(lcd_bufer);o++){lcd_bufer[o]=0x00;};
  for (uint8_t o=0;o!= sizeof(lcd_string);o++){lcd_string[o]=0x00;};

  // lcd_digit_clear();
  if(data > 999)
  {
    strncat(lcd_bufer,"999",4);
  }
  else
  {
    if(data < 10)
    {
      strncat(lcd_bufer,"  ",2);
    };
    
    if((data >= 10) && (data < 100))
    {
      strncat(lcd_bufer," ",1);
    };
    sprintf(lcd_string,"%u",data);
    strcat(lcd_bufer,lcd_string);
  };
  
  
  switch(place)
  {
  case LCD_CAR_LEVEL: 

    for ( char lcd_indx=0;lcd_indx < 3; lcd_indx++)
    {
      switch(lcd_bufer[lcd_indx])
      {
//-------------------------------------------------------------------------------------------------
      case ' ':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
         break;
          
        case 1:
          sim2_clr();
          break;
          
        case 2:
          sim3_clr();
          
          break;
          
        default:
          break;
        };
        break;
      case '0':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          s301;
          s201;
          s101;
          s001;
          s100;
          s300;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s203;
          s103;
          s003;
          s102;
          s302;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s205;
          s105;
          s005;
          s104;
          s304;
          break;
          
        default:
          break;
        };
        break;
//-------------------------------------------------------------------------------------------------
      case '1':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          s201;
          s101;
          break;
          
        case 1:
          sim2_clr();
          s203;
          s103;
          break;
          
        case 2:
          sim3_clr();
          s205;
          s105;
          break;
        default:
          break;
        };
        
        break;
//-------------------------------------------------------------------------------------------------        
       case '2':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          s301;
          s201;
          s200;
          s100;
          s001;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s203;
          s202;
          s102;
          s003;
          break;
          
        case 2:
          sim3_clr();
         s305;
         s205;
         s204;
         s104;
         s005;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------      
         case '3':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          s301;
          s201;
          s200;
          s101;
          s001;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s203;
          s202;
          s103;
          s003;
          break;
          
        case 2:
         sim3_clr();
         s305;
         s205;
         s204;
         s105;
         s005;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '4':
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          s300;
          s200;
          s201;
          s101;
          break;
          
        case 1:
         sim2_clr();
         s302;
         s202;
         s203;
         s103;
          break;
          
        case 2:
          sim3_clr();
          s304;
          s205;
          s105;
          s204;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '5':
        switch(lcd_indx)
        {
        case 0:  
          sim1_clr();
          s301;
          s300;
          s200;
          s101;
          s001;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s302;
          s202;
          s103;
          s003;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s304;
          s204;
          s105;
          s005;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '6':
        switch(lcd_indx)
        {
        case 0:  
          sim1_clr();
          s301;
          s300;
          s100;
          s001;
          s101;
          s200;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s302;
          s202;
          s103;
          s003;
          s102;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s304;
          s104;
          s005;
          s105;
          s204;
          
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '7':
        switch(lcd_indx)
        {
        case 0:  
          sim1_clr();
          s301;
          s201;
          s101;
          break;
          
        case 1:
          sim2_clr();
          s303;
          s203;
          s103;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s205;
          s105;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '8':
        switch(lcd_indx)
        {
        case 0:  
          sim1_clr();
          s301;
          s201;
          s101;
          s001;
          s100;
          s300;
          s200;
          break;
          
        case 1:
         sim2_clr();
         s303;
         s203;
         s103;
         s003;
         s102;
         s302;
         s202;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s205;
          s105;
          s005;
          s104;
          s304;
          s204;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '9':
        switch(lcd_indx)
        {
        case 0:  
          sim1_clr();
          s301;
          s201;
          s200;
          s300;
          s101;
          s001;
          break;
          
        case 1: 
          sim2_clr();
          s303;
          s203;
          s202;
          s302;
          s103;
          s003;
          break;
          
        case 2:
          sim3_clr();
          s305;
          s205;
          s204;
          s304;
          s105;
          s005;
          break;
        default:
          break;
        };
        break;
      case 0x00:
        switch(lcd_indx)
        {
        case 0:
          sim1_clr();
          break;
          
        case 1:
          sim2_clr();
          break;
          
        case 2:
          sim3_clr();
          break;
          
        default:
          break;
        };
     
 //----------------------------------------------------------------------------------------    
       };  
      
      
     };
   
    break;
 //===========================================END_OF_MAX_LEVEL===================================== 
 //===========================================MIN_LEVEL_SIMBOL_SINTEZ==============================
  case LCD_MAX_LEVEL: 
    for ( char lcd_indx=0;lcd_indx < 3; lcd_indx++)
    {
      switch(lcd_bufer[lcd_indx])
      {
//-------------------------------------------------------------------------------------------------
     case ' ':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          
          break;
          
        case 1: 
          sim5_clr();
          
          break;
          
        case 2:
          sim6_clr();
          
          break;
          
        default:
          break;
        };
         break;
      case '0':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          s307;
          s207;
          s107;
          s007;
          s106;
          s306;
          break;
          
        case 1: 
          sim5_clr();
          s309;
          s209;
          s109;
          s009;
          s108;
          s308;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s211;
          s111;
          s011;
          s110;
          s310;
          break;
          
        default:
          break;
        };
         break;
//-------------------------------------------------------------------------------------------------
      case '1':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          s207;
          s107;
          break;
          
        case 1: 
          sim5_clr();
          s209;
          s109;
          break;
          
        case 2:
          sim6_clr();
          s211;
          s111;
          break;
        default:
          break;
        };
        
        break;
//-------------------------------------------------------------------------------------------------        
       case '2':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          s307;
          s207;
          s206;
          s106;
          s007;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s209;
          s208;
          s108;
          s009;
          break;
          
        case 2:
         sim6_clr();
         s311;
         s211;
         s210;
         s110;
         s011;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------      
         case '3':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          s307;
          s207;
          s206;
          s107;
          s007;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s209;
          s208;
          s109;
          s009;
          break;
          
        case 2:
          sim6_clr();
         s311;
         s211;
         s210;
         s111;
         s011;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '4':
        switch(lcd_indx)
        {
        case 0:  
          sim4_clr();
          s306;
          s206;
          s207;
          s107;
          break;
          
        case 1:
          sim5_clr();
         s308;
         s208;
         s209;
         s109;
          break;
          
        case 2:
          sim6_clr();
          s310;
          s211;
          s111;
          s210;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '5':
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          s307;
          s306;
          s206;
          s107;
          s007;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s308;
          s208;
          s109;
          s009;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s310;
          s210;
          s111;
          s011;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '6':
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          s307;
          s306;
          s106;
          s007;
          s107;
          s206;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s308;
          s208;
          s109;
          s009;
          s108;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s310;
          s110;
          s011;
          s111;
          s210;
          
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '7':
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          s307;
          s207;
          s107;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s209;
          s109;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s211;
          s111;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '8':
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          s307;
          s207;
          s107;
          s007;
          s106;
          s306;
          s206;
          break;
          
        case 1:
          sim5_clr();
         s309;
         s209;
         s109;
         s009;
         s108;
         s308;
         s208;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s211;
          s111;
          s011;
          s110;
          s310;
          s210;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '9':
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          s307;
          s207;
          s206;
          s306;
          s107;
          s007;
          break;
          
        case 1:
          sim5_clr();
          s309;
          s209;
          s208;
          s308;
          s109;
          s009;
          break;
          
        case 2:
          sim6_clr();
          s311;
          s211;
          s210;
          s310;
          s111;
          s011;
          break;
        default:
          break;
        };
        break;
        case 0x00:
        switch(lcd_indx)
        {
        case 0:
          sim4_clr();
          break;
          
        case 1:
          sim5_clr();
          break;
          
        case 2:
          sim6_clr();
          break;
          
        default:
          break;
        };
     
 //----------------------------------------------------------------------------------------    
       };  
      
      
     };
   
    
    break;
     case LCD_MIN_LEVEL: 
    for ( char lcd_indx=0;lcd_indx < 3; lcd_indx++)
    {
      switch(lcd_bufer[lcd_indx])
      {
//-------------------------------------------------------------------------------------------------
      case ' ':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          
          break;
          
        case 1: 
          sim8_clr();
         
          break;
          
        case 2:
          sim9_clr();
          
          break;
          
        default:
          break;
        };
         break;
      case '0':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          s313;
          s213;
          s113;
          s013;
          s112;
          s312;
          break;
          
        case 1: 
          sim8_clr();
          s315;
          s215;
          s115;
          s015;
          s114;
          s314;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s217;
          s117;
          s017;
          s116;
          s316;
          break;
          
        default:
          break;
        };
         break;
//-------------------------------------------------------------------------------------------------
      case '1':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          s213;
          s113;
          break;
          
        case 1: 
          sim8_clr();
          s215;
          s115;
          break;
          
        case 2:
          sim9_clr();
          s217;
          s117;
          break;
        default:
          break;
        };
        
        break;
//-------------------------------------------------------------------------------------------------        
       case '2':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          s313;
          s213;
          s212;
          s112;
          s013;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s215;
          s214;
          s114;
          s015;
          break;
          
        case 2:
         sim9_clr();
         s317;
         s217;
         s216;
         s116;
         s017;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------      
         case '3':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          s313;
          s213;
          s212;
          s113;
          s013;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s215;
          s214;
          s115;
          s015;
          break;
          
        case 2:
          sim9_clr();
         s317;
         s217;
         s216;
         s117;
         s017;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '4':
        switch(lcd_indx)
        {
        case 0:  
          sim7_clr();
          s312;
          s212;
          s213;
          s113;
          break;
          
        case 1:
          sim8_clr();
         s314;
         s214;
         s215;
         s115;
          break;
          
        case 2:
          sim9_clr();
          s316;
          s217;
          s117;
          s216;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '5':
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          s313;
          s312;
          s212;
          s113;
          s013;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s314;
          s214;
          s115;
          s015;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s316;
          s216;
          s117;
          s017;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '6':
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          s313;
          s312;
          s112;
          s013;
          s113;
          s212;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s314;
          s214;
          s115;
          s015;
          s114;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s316;
          s116;
          s017;
          s117;
          s216;
          
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '7':
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          s313;
          s213;
          s113;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s215;
          s115;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s217;
          s117;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '8':
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          s313;
          s213;
          s113;
          s013;
          s112;
          s312;
          s212;
          break;
          
        case 1:
          sim8_clr();
         s315;
         s215;
         s115;
         s015;
         s114;
         s314;
         s214;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s217;
          s117;
          s017;
          s116;
          s316;
          s216;
          break;
        default:
          break;
        };
        break;
        
     
 //----------------------------------------------------------------------------------------    
         case '9':
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          s313;
          s213;
          s212;
          s312;
          s113;
          s013;
          break;
          
        case 1:
          sim8_clr();
          s315;
          s215;
          s214;
          s314;
          s115;
          s015;
          break;
          
        case 2:
          sim9_clr();
          s317;
          s217;
          s216;
          s316;
          s117;
          s017;
          break;
        default:
          break;
        };
        break;
        case 0x00:
        switch(lcd_indx)
        {
        case 0:
          sim7_clr();
          break;
          
        case 1:
          sim8_clr();
          break;
          
        case 2:
          sim9_clr();
          break;
          
        default:
          break;
        };
     
 //----------------------------------------------------------------------------------------    
       };  
      
      
     };
    break;
  default:
    break;
  };
};
    
    