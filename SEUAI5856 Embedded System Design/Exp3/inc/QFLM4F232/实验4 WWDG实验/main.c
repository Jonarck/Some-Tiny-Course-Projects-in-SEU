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
// �Ӻ�������˵����            	  �򵥵���ʱ����   
//-----------------------------------------------------------------------------
void delay (unsigned long ulTime)
{
    while (--ulTime != 0);                                             
}

//-----------------------------------------------------------------------------
// �Ӻ�������˵����            	  ��תLED��   
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
#define     KEY    GPIO_PIN_0                                            // ����KEY PG0                 

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
    lcd_init();	                  //��ʼ��LCD����

		PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);  //close DC motor[0][3]  close relay[5:7]

	
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);     // ʹ�ܿ��Ź���ʱ��            	 
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);     // ʹ��GPIO G��                
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);    //  ʹ��GPIO C��      
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);	//H 
	 GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE,0xFF);
   GPIOPadConfigSet(GPIO_PORTG_BASE, KEY, GPIO_STRENGTH_2MA,				
                     GPIO_PIN_TYPE_STD_WPU);
										 
	 GPIODirModeSet(GPIO_PORTG_BASE, KEY, GPIO_DIR_MODE_IN); // ��������KEYΪ����     
// 	 GPIOPadConfigSet(GPIO_PORTH_BASE, LED1, GPIO_STRENGTH_2MA,
//                      GPIO_PIN_TYPE_STD_WPU);
// 	 GPIODirModeSet(GPIO_PORTH_BASE, LED1, GPIO_DIR_MODE_OUT); //  ��������LED1Ϊ���   					
  	 //GPIOPadConfigSet(GPIO_PORTC_BASE, LED2, GPIO_STRENGTH_2MA,
     //                GPIO_PIN_TYPE_STD_WPU);
  	 //GPIODirModeSet(GPIO_PORTC_BASE, LED2, GPIO_DIR_MODE_OUT); // ��������LED2Ϊ��� 
     		 
  	 IntPrioritySet(INT_WATCHDOG,0x00);   // ���ÿ��Ź��ж����ȼ�Ϊ��    
  	 IntPrioritySet(INT_GPIOG,0x40);       // ����GPIO F�ڵ��ж����ȼ�Ϊ��
     GPIOIntTypeSet(GPIO_PORTG_BASE, KEY,  GPIO_LOW_LEVEL); //  ����KEY�жϴ���ΪIO���½��ش�����
  	 GPIOPinIntEnable(GPIO_PORTG_BASE, KEY);             //ʹ��KEY�ж�                
		 IntEnable(INT_GPIOG);               // ʹ��GPIO G���ж�            
  	 IntEnable(INT_WATCHDOG);             //  ʹ�ܿ��Ź��ж�             
  	 WatchdogReloadSet(WATCHDOG_BASE, 100000000);  // ���ÿ��Ź���ʱ��������ֵ    
  	 WatchdogResetEnable(WATCHDOG_BASE);   // ʹ�ܿ��Ź���ʱ���ĸ�λ���� 
   	 WatchdogEnable(WATCHDOG_BASE);     //  ʹ�ܿ��Ź���ʱ�����ж�   
     WatchdogLock(WATCHDOG_BASE);      // ʹ�ܿ��Ź���ʱ������������ 
		 
	
		LCD_Clear(Black);	
		LCD_DrawRectangle(0,5, 479, 271);
		LCD_DrawRectangle(5,18, 474, 266);
		LCD_ShowString(150, 6, "--TM4C123G Platform--");  // ʵ�鿪��ƽ̨	
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