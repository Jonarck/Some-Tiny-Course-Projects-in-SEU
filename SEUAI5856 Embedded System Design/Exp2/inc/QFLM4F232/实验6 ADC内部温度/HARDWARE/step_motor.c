#include "step_motor.h"

u8 F_Rotation[8]={0x08,0x0C,0x04,0x06,0x02,0x03,0x01,0x09};		//

void STEP_Motor_delay(u32 tim)
{
	unsigned char i;
	for(;tim>0;tim--)
	 for(i=0;i<180;i++);
}

void STEP_Motor_GPIO_Init(void)
{
	// Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	
    ROM_GPIOPinTypeGPIOOutput(IN1_GPIO, IN1_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(IN2_GPIO, IN2_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(IN3_GPIO, IN3_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(IN4_GPIO, IN4_GPIO_PIN);

		IN3_ONOFF(IN3_GPIO_PIN);
		IN4_ONOFF(IN4_GPIO_PIN);
}

void BUJIN_port(u8 dat)
{
	//unsigned char dat=0x09;
		if((dat&0x08)==0x00)
		IN2_ONOFF(0);
	else IN2_ONOFF(IN2_GPIO_PIN);
		
		if((dat&0x04)==0x00)
		IN4_ONOFF(0);
	else IN4_ONOFF(IN4_GPIO_PIN);		
		
			if((dat&0x02)==0x00)
		IN3_ONOFF(0);
	else IN3_ONOFF(IN3_GPIO_PIN);
		
		if((dat&0x01)==0x00)
		IN1_ONOFF(0);
	else IN1_ONOFF(IN1_GPIO_PIN);
}

void STEP_Motor_test(void)
{
	u8 i,j;

	STEP_Motor_GPIO_Init();	

   for(j=0;j<180;j++)
		for(i = 0; i < 8; i ++)
		{
			BUJIN_port(F_Rotation[i]);
			STEP_Motor_delay(40);		//1ms
		}
		BUJIN_port(0x00); //        8214
}
