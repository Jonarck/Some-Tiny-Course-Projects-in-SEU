#include "lm4f232h5qd.h"
#include "inc/hw_sysctl.h"
#include <stdbool.h>
#include <stdint.h>
#include <Stdlib.h>
#include <String.h>

#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "inc/hw_ints.h"			  //???????
#include "driverlib/interrupt.h"	  //
#define    SYS_CLOCK_KHZ   80000
void sys_cfg(unsigned long ulConfig);
void sysclock_cfg();
void delay (unsigned long ulTime);
int main(void) {
	sysclock_cfg();
	
	SYSCTL_RCGCGPIO_R = 0x80;  //GPIOH

	SysCtlDelay(16);
	GPIO_PORTH_LOCK_R = 0x4C4F434B;
	GPIO_PORTH_DIR_R = 0XFF;
	GPIO_PORTH_AFSEL_R = 0x00;
	GPIO_PORTH_DEN_R = 0xFF;
	GPIO_PORTH_CR_R=0XFF;
	while(1)
	{
		GPIO_PORTH_DATA_R = 0x80;
		delay(1000000);
		GPIO_PORTH_DATA_R = 0x40;
		delay(1000000);
		GPIO_PORTH_DATA_R = 0x20;
		delay(1000000);
		GPIO_PORTH_DATA_R = 0x10;
		delay(1000000);
		GPIO_PORTH_DATA_R = 0x00;
		delay(1000000);
	}
}

  
void sysclock_cfg()
{
	//RCC :0000 0000 0000 0100 1110 0000 0000 0000
	//RCC2:1100 0001 0000 0000 0000 0000 0000 0000   C0 01 00 00
	uint32_t ui32Delay, ui32RCC, ui32RCC2;
	//
	// Get the current value of the RCC and RCC2 registers.
	//
	ui32RCC = SYSCTL_RCC_R;
	ui32RCC2 = SYSCTL_RCC2_R;
	//
	// Bypass the PLL and system clock dividers for now.
	//
	ui32RCC |= SYSCTL_RCC_BYPASS;//??PLL
	ui32RCC &= ~(SYSCTL_RCC_USESYSDIV);//??DIV
	ui32RCC2 |= SYSCTL_RCC2_BYPASS2;//??PLL

  //
	// Write the new RCC value.
	//
	SYSCTL_RCC_R = ui32RCC;
	SYSCTL_RCC2_R = ui32RCC2;

	//
	//2启动主时钟并等待稳定
		SYSCTL_RCC_R  &= ~(0x01);

		for(ui32Delay = 524288; ui32Delay > 0; ui32Delay--)
		{
		}

		//3选择时钟源作为主时钟，并设置时钟频率
		ui32RCC  = SYSCTL_RCC_R;
		ui32RCC2 = SYSCTL_RCC2_R;
		ui32RCC  &= ~(0x000007c0 | 0x00000030);   //?????????????
		ui32RCC  |= 0x00000540;    //??????

		SYSCTL_RCC_R  = ui32RCC;    //??????
		SYSCTL_RCC2_R &= ~(0x00000070);  //???????
		SYSCTL_RCC2_R  |= (0x80000000);  //??RCC2
		for(ui32Delay = 32768; ui32Delay > 0; ui32Delay--)
		{
		}

		//4允许PLL??
		SYSCTL_MISC_R |= 0x00000040; //??PLL????

		SYSCTL_RCC2_R &= ~(0x00002000);  //PLL??
		SYSCTL_RCC_R  &= ~(0x00002000);  //pll??



		//5设置分频器值
		ui32RCC  = SYSCTL_RCC_R;
		ui32RCC &= ~(0x07800000 | 0x00400000);
		ui32RCC |= 0x07800000;
		ui32RCC2 = SYSCTL_RCC2_R;
		ui32RCC2 &= ~(0x1f800000 | 0x00400000);  //????
		ui32RCC2 |= (0x41000000);  //??DIV400 ?????
		//SYSCTL_RCC2_R = ui32RCC2;

		//6???PLL???,???PLL
		for(ui32Delay = 32768; ui32Delay > 0; ui32Delay--)
		{
			if(SYSCTL_RIS_R & 0x00000040)
			{
				break;
			}

		}
		ui32RCC  &= ~(0x00000800);
		ui32RCC2 &= ~(0x00000800);

		SYSCTL_RCC_R  = ui32RCC;
		SYSCTL_RCC2_R = ui32RCC2;
		// = ui32RCC2;

		for(ui32Delay = 0; ui32Delay < 10000; )
		{
			ui32Delay++;
		}

}

int GPIO_cfg()
{
	int waittime;

	SYSCTL_RCGCGPIO_R = 0x01;  //??GPIOA
	//??GPIO????
	waittime = 0;
	while((SYSCTL_PRGPIO_R != 0X3f))
	{
		waittime++;
		if(waittime > 100000)
		{
			//??????
			return 1;
		}
	}
	GPIO_PORTA_DIR_R = 0X01;//??GPIO????,????????
	GPIO_PORTA_AFSEL_R = 0x00;//??GPIO????

}

void watchdog_fun()
{}
/*
int i2c_cfg()
{
	SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;//??I2C??0
	SYSCTL_RCGCGPIO_R |= 0x02;//??GPIO棗PB??
	GPIO_PORTB_AFSEL_R |= 0x0c;//??PB2,PB3????
	GPIO_PORTB_PCTL_R |= 0x00003300;
	I2C0_MASTER_MCR_R |= I2C_MCR_MFE;//I2C Master Function Enable
	I2C0_MASTER_MTPR_R |= 0x00000009;//????
	I2C0_MASTER_MSA_R |= 0x46;//????0x23,??
}
	*/
	void delay (unsigned long ulTime)
{
    while (--ulTime != 0);                                             
}
