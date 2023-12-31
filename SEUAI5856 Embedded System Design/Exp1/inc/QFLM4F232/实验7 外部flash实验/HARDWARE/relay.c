#include "relay.h"
#include "lcd.h"

//OUTE5 E6 E7 -> 3 RELAY

void Relay_delay_tim(u32 tim)
{
	unsigned char i;
	for(;tim>0;tim--)
	 for(i=0;i<250;i++);
}

void Relay_Gpio_Init(void)
{

};

void Relay_test(void)
{
	PCF8574_Single_WriteI2C(PCF8574T_E, 0xa0);			
	Relay_delay_tim(4000);	
	
	PCF8574_Single_WriteI2C(PCF8574T_E, 0x80);			
	Relay_delay_tim(4000);	

	PCF8574_Single_WriteI2C(PCF8574T_E, 0);			
	Relay_delay_tim(4000);	
	
	PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);		 					
	Relay_delay_tim(4000);
}
