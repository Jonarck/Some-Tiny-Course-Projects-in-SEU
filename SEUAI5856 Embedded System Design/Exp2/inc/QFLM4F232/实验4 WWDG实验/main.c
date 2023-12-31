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

//-----------------------------------------------------------------------------
// 子函数功能说明：            	  简单的延时函数   
//-----------------------------------------------------------------------------
void delay (unsigned long ulTime)
{
    while (--ulTime != 0);                                             
}

//-----------------------------------------------------------------------------
// 子函数功能说明：            	  反转LED灯   
//-----------------------------------------------------------------------------
void Watchdog_Timer (void)
{
	while(1){	
		PCF8574_Single_WriteI2C(PCF8574T_C, 0xE6);  //C1                                                                  
		delay(100000);
		PCF8574_Single_WriteI2C(PCF8574T_C, 0xF6);  //C1
		delay(100000);
		 	 }
}


static uint8_t read_val = 0;
#define     KEY    GPIO_PIN_0                                            // 定义KEY PG0                 

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

	
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);     // 使能看门狗定时器            	 
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);     // 使能GPIO G口                
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);    //  使能GPIO C口      
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);	//H 
	 GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE,0xFF);
   GPIOPadConfigSet(GPIO_PORTG_BASE, KEY, GPIO_STRENGTH_2MA,				
                     GPIO_PIN_TYPE_STD_WPU);
										 
	 GPIODirModeSet(GPIO_PORTG_BASE, KEY, GPIO_DIR_MODE_IN); // 设置连接KEY为输入     
// 	 GPIOPadConfigSet(GPIO_PORTH_BASE, LED1, GPIO_STRENGTH_2MA,
//                      GPIO_PIN_TYPE_STD_WPU);
// 	 GPIODirModeSet(GPIO_PORTH_BASE, LED1, GPIO_DIR_MODE_OUT); //  设置连接LED1为输出   					
  	 //GPIOPadConfigSet(GPIO_PORTC_BASE, LED2, GPIO_STRENGTH_2MA,
     //                GPIO_PIN_TYPE_STD_WPU);
  	 //GPIODirModeSet(GPIO_PORTC_BASE, LED2, GPIO_DIR_MODE_OUT); // 设置连接LED2为输出 
     		 
  	 IntPrioritySet(INT_WATCHDOG,0x00);   // 设置看门狗中断优先级为高    
  	 IntPrioritySet(INT_GPIOG,0x40);       // 设置GPIO F口的中断优先级为低
     GPIOIntTypeSet(GPIO_PORTG_BASE, KEY,  GPIO_LOW_LEVEL); //  设置KEY中断触发为IO口下降沿触发发
  	 GPIOPinIntEnable(GPIO_PORTG_BASE, KEY);             //使能KEY中断                
		 IntEnable(INT_GPIOG);               // 使能GPIO G口中断            
  	 IntEnable(INT_WATCHDOG);             //  使能看门狗中断             
  	 WatchdogReloadSet(WATCHDOG_BASE, 100000000);  // 设置看门狗定时器的重载值    
  	 WatchdogResetEnable(WATCHDOG_BASE);   // 使能看门狗定时器的复位功能 
   	 WatchdogEnable(WATCHDOG_BASE);     //  使能看门狗定时器的中断   
     WatchdogLock(WATCHDOG_BASE);      // 使能看门狗定时器的锁定机制 
		 
	
		LCD_Clear(Black);	
		LCD_DrawRectangle(0,5, 479, 271);
		LCD_DrawRectangle(5,18, 474, 266);
		LCD_ShowString(150, 6, "--TM4C123G Platform--");  // 实验开发平台	
		LCD_ShowString(150,60, "watchdog START!");	
	
		//UARTprintf("hello!welcome to TM4C123G");
	
		while(1)	
		{				
			PCF8574_Single_WriteI2C(PCF8574T_C, 0xF4);  //C1
			delay(100000);	
			PCF8574_Single_WriteI2C(PCF8574T_C, 0xF6);  //C1
			delay(100000);		
			GPIOPinWrite(GPIO_PORTH_BASE,0x10,1);
			delay(100000);
			GPIOPinWrite(GPIO_PORTH_BASE,0x10,0);
			delay(100000);
		}
}