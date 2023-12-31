#include "DC_motor.h"

//DC_Motor1A : PE0
//DC_Motor1B : PB14

void DC_Motor_delay(u32 tim)
{
	unsigned char i;
	for(;tim>0;tim--)
	 for(i=0;i<250;i++);
}

void DC_Motor_GPIO_Init(void)
{
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
    ROM_GPIOPinTypeGPIOOutput(MOTOR_PE0_BASE, 	MOTOR_PE0_PIN);
}

void DC_Motor_test(void)
{
	PCF8574_Single_WriteI2C(PCF8574T_E, 0xe8);  //C1	
	DC_Motor_delay(8000);	
	PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);  //C1	
}