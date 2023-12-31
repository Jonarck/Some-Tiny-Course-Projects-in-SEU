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
// �Ӻ�������˵����            	  �򵥵���ʱ����   
//-----------------------------------------------------------------------------
void delay (unsigned long ulTime)
{
    while (--ulTime != 0);                                             
}

static uint8_t read_val = 0;
#define     KEY    GPIO_PIN_0                                            // ����KEY PG0                 

int main(void)
{
    tContext sContext;
    tRectangle sRect;
	  char cText[18];
    unsigned long ulADC0_Value[1];  // ���������ȡ ADC FIFO
    unsigned long ulTemp_ValueC;
    unsigned long ulTemp_ValueF;   // �����¶�ת������
	
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
    lcd_init();	                  //��ʼ��LCD����

		PCF8574_Single_WriteI2C(PCF8574T_E, 0xe0);  //close DC motor[0][3]  close relay[5:7]
		 
	
		LCD_Clear(Black);	
		LCD_DrawRectangle(0,5, 479, 271);
		LCD_DrawRectangle(5,18, 474, 266);
		LCD_ShowString(150, 6, "--TM4C123G Platform--");  // ʵ�鿪��ƽ̨	

		LCD_ShowString(150,60, "Note the UART3 information");	
    UARTprintf("ADC ->\n");
    UARTprintf("  Type: Internal Temperature Sensor\n");
    UARTprintf("  Samples: One\n");
    UARTprintf("  Update Rate: 250ms\n");
    UARTprintf("  Input Pin: Internal temperature sensor\n\n");	   // ������ʾ

		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);	 // ADC0�����ʼ��
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);// ����ADC0����������3��ADC���������������ȼ�Ϊ0
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                             ADC_CTL_END);// �¶ȴ��У��ж�ʹ�ܣ����н���ѡ����ADCͨ��
    ADCSequenceEnable(ADC0_BASE, 3); //ʹ��ADC����  
    ADCIntClear(ADC0_BASE, 3);//ADC�ж����

		while(1)	
		{				
        ADCProcessorTrigger(ADC0_BASE, 3); //ADC����������
        while(!ADCIntStatus(ADC0_BASE, 3, false))//��ȡ�ж�״̬
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 3, ulADC0_Value); //��ȡADC�������� 
        ulTemp_ValueC = ((1475 * 4095) - (2250 * ulADC0_Value[0])) / 40950;// �����¶� C
        ulTemp_ValueF = ((ulTemp_ValueC * 9) + 160) / 5;  // �����¶� F
				UARTprintf("Temperature = %3d*C or %3d*F\r", ulTemp_ValueC,
                   ulTemp_ValueF);		 //�����¶���ʾ
				usprintf(cText, "Temperature:  %d C",ulTemp_ValueC);
				LCD_ShowString(150,100, cText);	
        //GrContextFontSet(&sContext, &g_sFontCm20);
				//GrStringDraw(&sContext, cText, -1, 110, 50, 1);	 //Һ������ʾ�¶�
        SysCtlDelay(SysCtlClockGet() / 6);   // ʱ���ӳ�			
		}
}