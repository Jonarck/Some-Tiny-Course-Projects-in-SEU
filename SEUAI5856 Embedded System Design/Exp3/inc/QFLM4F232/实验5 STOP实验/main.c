#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "inc/hw_ints.h"			  //???????
#include "driverlib/interrupt.h"	  //
#include "driverlib/hibernate.h"
#include "utils/ustdlib.h"

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

    unsigned long ulStatus, ulBBData=0, ulCount;
    char cText[15];	
	
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
		//LCD_ShowString(150,60, "watchdog START!");	
	
		//UARTprintf("hello!welcome to TM4C123G");
	
    // �����ʼ������ģʽ
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
	//ѯ�ʿ��ƼĴ�������ģ���Ƿ��Ѽ���
	if(HibernateIsActive())
	{
		//
		// ��ȡ����ģ��ĵ�ǰԭʼ�ж�
		//
		ulStatus=HibernateIntStatus(0);


		if(ulStatus & HIBERNATE_INT_PIN_WAKE)
		{
			//
			// ��������WAKE�ܽ��жϲ�����.
			//
			//GrContextFontSet(&sContext, &g_sFontCm20);
			//GrStringDraw(&sContext,"Wake Cause: WK", -1,50, 30, 1);
			LCD_ShowString(150,150, "Wake Cause: WK");	
		}

		else if (ulStatus & HIBERNATE_INT_RTC_MATCH_0)
		{
			//
			// ��������RTC��ʱ������
			//
			//GrStringDraw(&sContext,"Wake Cause: RTC", -1,50, 30, 1);
			LCD_ShowString(150,150, "Wake Cause: RTC");	
		}

		else
		{
			//
			// �������ɸ�λ��������.
			//
			//GrStringDraw(&sContext,"Wake Cause: RST", -1,50, 30, 1);
			LCD_ShowString(150,150, "Wake Cause: RST");
		}

		//
		// �Ӷ���ģ��ķ���ʧ�Դ洢���ж�ȡһ������
		//
		if(ulStatus & (HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_RTC_MATCH_0))
		{
			HibernateDataGet(&ulBBData, 1);
		}
	}

	//
	// ʹ�ܶ���ģ��ʱ��
	//
	HibernateEnableExpClk(SysCtlClockGet());


	//
	// ��Ļ��ʾ�û�����
	//
    usprintf(cText, "NV Data   : %d", ulBBData);
		//GrStringDraw(&sContext, cText, -1, 50, 90, 1);
		LCD_ShowString(150,100, cText);

    //
    // �ȴ�5����붬��ģʽ
    //
    for (ulCount=5; ulCount>0; ulCount--)
    {
			usprintf(cText, "Enter Hib : %d s", ulCount);
			//GrStringDraw(&sContext, cText, -1, 50, 60, 1);
			LCD_ShowString(150,60, cText);
			SysCtlDelay(8333333);
    }

    //
    // �û������ԼӼ�
    //
    ulBBData++;
    HibernateDataSet(&ulBBData, 1);

	// >>>>>>>>>>>> SECTION: A
    //
    // Set RTC counter = 0, enable RTC, set RTC match 0 register
    // for 5 seconds.
    //
    /*
    HibernateRTCSet(0);
    HibernateRTCEnable();
    HibernateRTCMatch0Set(5);
	*/

	// >>>>>>>>>>> SECTION: B
    //
    // ���ⲿ���ѹܽ���Чʱ����
    //
    HibernateWakeSet(HIBERNATE_WAKE_PIN);

    //
    // ������ģʽ
    //
    HibernateRequest();
    SysCtlDelay(5000);	
		while(1)	
		{				
			
		}
}