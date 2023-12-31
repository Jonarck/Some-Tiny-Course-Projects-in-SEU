#include "utils/ustdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "grlib/grlib.h"
#include "driverlib/fpu.h"


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

static uint8_t read_val = 0;
#define     KEY    GPIO_PIN_0                                            // 定义KEY PG0                 

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

		LCD_ShowString(150,60, "Note the UART3 information");	
    UARTprintf("ADC ->\n");
    UARTprintf("  Type: Internal Temperature Sensor\n");
    UARTprintf("  Samples: One\n");
    UARTprintf("  Update Rate: 250ms\n");
    UARTprintf("  Input Pin: Internal temperature sensor\n\n");	   // 串口显示

		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);	 // ADC0外设初始化
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);// 配置ADC0，采样序列3，ADC处理器触发，优先级为0
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                             ADC_CTL_END);// 温度传感，中断使能，对列结束选择，无ADC通道
    ADCSequenceEnable(ADC0_BASE, 3); //使能ADC采样  
    ADCIntClear(ADC0_BASE, 3);//ADC中断清除

		while(1)	
		{				
        ADCProcessorTrigger(ADC0_BASE, 3); //ADC处理器触发
        while(!ADCIntStatus(ADC0_BASE, 3, false))//获取中断状态
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 3, ulADC0_Value); //获取ADC采样数据 
        ulTemp_ValueC = ((1475 * 4095) - (2250 * ulADC0_Value[0])) / 40950;// 计算温度 C
        ulTemp_ValueF = ((ulTemp_ValueC * 9) + 160) / 5;  // 计算温度 F
				UARTprintf("Temperature = %3d*C or %3d*F\r", ulTemp_ValueC,
                   ulTemp_ValueF);		 //设置温度显示
				usprintf(cText, "Temperature:  %d C",ulTemp_ValueC);
				LCD_ShowString(150,100, cText);	
        //GrContextFontSet(&sContext, &g_sFontCm20);
				//GrStringDraw(&sContext, cText, -1, 110, 50, 1);	 //液晶屏显示温度
        SysCtlDelay(SysCtlClockGet() / 6);   // 时钟延迟			
		}
}