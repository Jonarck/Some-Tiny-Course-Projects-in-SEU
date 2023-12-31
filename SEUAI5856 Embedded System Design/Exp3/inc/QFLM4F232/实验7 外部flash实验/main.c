#include "utils/ustdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "driverlib/fpu.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "drivers/ssi_hw.h"

#include "hardware/lcd.h"
#include "hardware/PCF8574.h"
#include "hardware/key.h"
#include "flash.h" 
 
//enable uart interrupt
#define UART_BUFFERED

//-----------------------------------------------------------------------------
// 子函数功能说明：            	  简单的延时函数   
//-----------------------------------------------------------------------------
void delay (unsigned long ulTime)
{
    while (--ulTime != 0);                                             
}

static uint8_t read_val = 0;
#define     KEY    GPIO_PIN_0                                            // 定义KEY PG0                 

int main(void)
{
    tContext sContext;
    tRectangle sRect;
		
		unsigned char i = 0;
	
		char b_flash[10] = {0,1,2,3,4,5,6,7,8,9};
		char a_flash[10] = {0};

    FPUEnable();						
    FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

		InitConsole(3);	  

		PCF8574_I2C_GPIO_Config();
		//KEY_Init();
    //lcd_init();	                  //初始化LCD驱动

		PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);  //close DC motor[0][3]  close relay[5:7]

		UARTprintf("SPI Flash test\n");

#if 1
		SPI_Flash_Init();			  //初始化Flash;	
		SPI_Flash_Erase_Sector(0);
		
		
		UARTprintf("Write: 0,1,2,3,4,5,6,7,8,9\n");
		SPI_Flash_Write(b_flash,0,10);
		SPI_Flash_Read(a_flash, 0, 10); 	
		UARTprintf("read:");	
		
		for(i = 0; i < 10; i++)
		 UARTprintf(" %02x", a_flash[i]);
		UARTprintf("\n"); 
		UARTprintf("Test Complete!\n"); 
#endif		
		while(1)
		{
 	
		}
}