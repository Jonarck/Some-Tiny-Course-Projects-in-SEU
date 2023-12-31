#include "../inc/hw_ints.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../driverlib/debug.h"
#include "../driverlib/fpu.h"
#include "../driverlib/interrupt.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/timer.h"
#include "../driverlib/rom.h"
#include "../grlib/grlib.h"


#include "../hardware/lcd.h"
#include "../hardware/PCF8574.h"
#include "../hardware/key.h"
 
//enable uart interrupt
#define UART_BUFFERED

//-----------------------------------------------------------------------------
//中断标志
//-----------------------------------------------------------------------------
unsigned long g_ulFlags;

//-----------------------------------------------------------------------------
//显示指针
//-----------------------------------------------------------------------------
tContext g_sContext;

							   
 void Delay10ms(void)
{
		
   int i=10000000;  
   while(i) 
   { 
     i--; 
   }  
}

//-----------------------------------------------------------------------------
// 定时器0中断
//-----------------------------------------------------------------------------
void
Timer0IntHandler(void)
{
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);// 清除时钟中断
    HWREGBITW(&g_ulFlags, 0) ^= 1;// 定时器切换标志
    ROM_IntMasterDisable();
		LCD_ShowString(150, 60, "--Timer0 interrupt!--");
    //GrStringDraw(&g_sContext, (HWREGBITW(&g_ulFlags, 0) ? "1" : "0"), -1, 98,
    //             26, 1);// 显示屏上的更新中断状态
    ROM_IntMasterEnable();
		Delay10ms();
    LCD_ShowString(150, 100, "                     ");
}
//-----------------------------------------------------------------------------
// 定时器1中断
//-----------------------------------------------------------------------------
void
Timer1IntHandler(void)
{
    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);    // 清除时钟中断
    HWREGBITW(&g_ulFlags, 1) ^= 1;  // 定时器切换标志
    ROM_IntMasterDisable();
    LCD_ShowString(150, 100, "--Timer1 interrupt!--");
		//GrStringDraw(&g_sContext, (HWREGBITW(&g_ulFlags, 1) ? "1" : "0"), -1, 98,
   //              46, 1); // 显示屏上的更新中断状态
    ROM_IntMasterEnable();
		Delay10ms();
    LCD_ShowString(150, 100, "                     ");	
}




int main(void)
{
    tContext sContext;
    tRectangle sRect;
	  char cText[18];
    unsigned long ulADC0_Value[1];  // 设置数组读取 ADC FIFO
    unsigned long ulTemp_ValueC;
    unsigned long ulTemp_ValueF;   // 设置温度转换变量
	
    unsigned long ulStatus, ulBBData=0, ulCount;
//    char cText[15];	
	
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

		//LCD_ShowString(150,100, "Note the UART3 information");	


		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);   //	使能外设
    ROM_IntMasterEnable();// 开总中断
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);// 配置2个32位周期定时器。
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() * 10);
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ROM_SysCtlClockGet() * 5);// 配置2个32位周期定时器的装载值。
    ROM_IntEnable(INT_TIMER0A);
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // 设置定时器超时中断。
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);  // 使能定时器0和1


		while(1)	
		{				
			
		}
}