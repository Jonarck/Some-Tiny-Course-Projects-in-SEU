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

void GPIO_INT_G(void)
 {
   unsigned char ucVal;
   unsigned long ulStatus;
   ulStatus = GPIOPinIntStatus(GPIO_PORTG_BASE, true); // ??????
   GPIOPinIntClear(GPIO_PORTG_BASE, ulStatus); // ??????,??
   if (ulStatus & GPIO_PIN_0) // ??KEY???????
	 {
	 //ucVal = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7); // ??LED
	// GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, ~ucVal);
		PCF8574_Single_WriteI2C(PCF8574T_C, 0xE6);  //C1
	 SysCtlDelay(10*(SysCtlClockGet()/ 3000)); // ???10ms,??????
	 while (GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_0) == 0x00); // ??KEY??
		SysCtlDelay(10*(SysCtlClockGet()/ 3000)); // ???10ms,??????
	 PCF8574_Single_WriteI2C(PCF8574T_C, 0xF6);
	}
 }

static uint8_t read_val = 0;

 //KEY: PG0
 //LED: OUTC1
 
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

	
	  //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	  // 使能外设GPIOC
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);	  // 使能外设GPIOG
		
      GPIOPadConfigSet(GPIO_PORTG_BASE,GPIO_PIN_0,
                     GPIO_STRENGTH_2MA,
                    GPIO_PIN_TYPE_STD_WPU);// 设置PG0为2MA，上拉输入
										
	  //GPIODirModeSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_DIR_MODE_OUT);	//PH4 设为输出
	  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);	//PF5 设为输入
	  GPIOIntTypeSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);		// 按键中断设为低电平触发
	  GPIOPinIntEnable(GPIO_PORTG_BASE, GPIO_PIN_0); // 使能KEY所在管脚的中断	
    IntEnable(INT_GPIOG); // 使能GPIOG端口中断
    IntMasterEnable(); // 使能处理器中断	
	
		LCD_Clear(Black);	
		LCD_DrawRectangle(0,5, 479, 271);
		LCD_DrawRectangle(5,18, 474, 266);
		LCD_ShowString(150, 6, "--TM4C123G Platform--");  // 实验开发平台	
		LCD_ShowString(150,60, "Press K2,and note LED!");	
	
		while(1)	
		{				
			
		}
}