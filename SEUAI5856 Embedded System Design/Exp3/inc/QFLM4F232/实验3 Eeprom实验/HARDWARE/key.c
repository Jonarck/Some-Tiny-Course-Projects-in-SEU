#include "key.h"
							   
 void Delay5ms(void)
{
		
   int i=50000;  
   while(i) 
   { 
     i--; 
   }  
}

void KEY_Init(void)
{ 
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	
	GPIOPadConfigSet(KEY_PG0_BASE,KEY_PG0_PIN,
                     GPIO_STRENGTH_2MA,
                    GPIO_PIN_TYPE_STD_WPU);
	
  GPIODirModeSet(KEY_PG0_BASE, KEY_PG0_PIN, GPIO_DIR_MODE_IN);
}

static uint8_t key_flag = 0;
static uint8_t read_val = 0;


//KEY: OUTC6 OUTC7 PG0
//LED: OUTC1    C2  C4
void KEY_Test(void)
{
	while(key_flag != 0x0D)
	{
		//if(strstr((const char*)gsm_ch, "OK"))
		//	LCD_ShowString(200, 100, "GSM_OK!"); 
		
		if(KEY1 == 0)	
		{
			Delay5ms();
			if(KEY1 == 0)
			{
				key_flag |= 0x01;
				PCF8574_Single_WriteI2C(PCF8574T_C, 0xE6);  //C1
			}
		}

		read_val = PCF8574_Single_ReadI2C((PCF8574T_C) | 0x01);
		if((read_val & 0x80) == 0)	//KEY3 : OUTC7
		{		
				key_flag |= 0x04;

				//UARTprintf(mode_U);
				//Delay5ms();
				
				PCF8574_Single_WriteI2C(PCF8574T_C, 0xE4);	//COUTC2
		}
		
		read_val = PCF8574_Single_ReadI2C((PCF8574T_C) | 0x01);		
		if((read_val & 0x40) == 0)	//KEY4: OUTC6
		{
			
			key_flag |= 0x08;
			
			//UARTprintf(next);			//MP3 TEST
			PCF8574_Single_WriteI2C(PCF8574T_C, 0xE0);	//on
		}		
	}
}
