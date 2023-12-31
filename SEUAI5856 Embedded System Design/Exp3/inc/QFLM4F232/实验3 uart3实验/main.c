#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "inc/hw_ints.h"			  //???????
#include "driverlib/interrupt.h"	  //


#include "hardware/lcd.h"
#include "hardware/PCF8574.h"
#include "hardware/key.h"
 
//enable uart interrupt
#define UART_BUFFERED
 
void Delay(unsigned int x)
{
    unsigned int j,t;
    while(x--){
        for(j=0;j<190;j++)
			    for(t=0;t<180;t++);
        }   
}

static uint8_t read_val = 0;

int main(void)
{
    tContext sContext;
    tRectangle sRect;
	
		uint32_t i = 0;
	
    FPUEnable();						
    FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

		InitConsole(3);	  
		//UART2_Int_Config();	
	
		PCF8574_I2C_GPIO_Config();
		KEY_Init();
    lcd_init();	                  //初始化LCD驱动

		PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);  //close DC motor[0][3]  close relay[5:7]
	
		LCD_Clear(Black);	
		LCD_DrawRectangle(0,5, 479, 271);
		LCD_DrawRectangle(5,18, 474, 266);
		LCD_ShowString(150, 6, "--TM4C123G Platform--");  // 实验开发平台	
		LCD_ShowString(150,60, "Note the uart3 information!");	
	
		UARTprintf("hello!welcome to TM4C123G");
	
		while(1)	
		{				
			
		}
}