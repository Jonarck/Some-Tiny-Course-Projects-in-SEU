//#include "uart.h"
#include "utils/uartstdio.h"
#include "sys.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "lcd.h"

//-----------------------------------------------------------------------------
// 子函数功能说明：            	  串口功能设置  
//5 OK	
//4 OK
//3 OK
//2  		PG5 PG4
//-----------------------------------------------------------------------------
void InitConsole(u8 num)
{
	if(2 == num)	//RFID
	{		
		//UART2 TXD5:PG5 RXD5:PG4	
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);	// 使能GPIO外设
    GPIOPinConfigure(GPIO_PG4_U2RX);				//	配置串口接收管脚
    GPIOPinConfigure(GPIO_PG5_U2TX);				//	 配置串口发送管脚
    GPIOPinTypeUART(GPIO_PORTG_BASE, GPIO_PIN_5 | GPIO_PIN_4);//把GPIO功能复用为串口
    UARTStdioInit(2, 115200);	   //设置串口属性
	}
	
	if(3 == num)				//MP3
	{
		//UART3 PC7:TXD3 PC6:RXD3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	// 使能GPIO外设
    GPIOPinConfigure(GPIO_PC6_U3RX);				//	配置串口接收管脚
    GPIOPinConfigure(GPIO_PC7_U3TX);				//	 配置串口发送管脚
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//把GPIO功能复用为串口
    UARTStdioInit(3, 9600);	   //设置串口属性	
	}
}

char Character[1] = {0};

char Date_Buf[10] = {0};
char BufferA[50] = {0};
uint8_t DBufferFlag = 0;
static unsigned char BufferCount = 0, temp = 0;

void UART2_Int_Handler(void)
{
    //中断标志的读取、处理和清除
    unsigned long ulStatus;
		uint8_t ch = 0, i = 0;
	
	char Rfid_Id[9];
	char Rfid_Value[35];	
	
    IntMasterDisable();//使能全局中断
    ulStatus = UARTIntStatus(UART2_BASE, true);
    UARTIntClear(UART2_BASE, ulStatus);
 
    //根据中断标志处理相关事件
     if(ulStatus & UART_INT_RX)
     {
			 	//LCD_ShowString(150,60, "UART_RX OK!");
			  for(i = 0; i < 48; i++)
        {
					 BufferA[i] = UARTCharGet(UART2_BASE);	
					 BufferCount = BufferCount + 1;
        }
				UARTprintf(BufferA);

				if(BufferCount > 48)
				{
					DBufferFlag = 1;
					BufferCount = 0;
				}
				else
				{
					DBufferFlag=0;
				}			 
#if 1
//如下代码放在外置函数中不能正常执行
	while(BufferCount<48);
		for(i=0;i<9;i++)
			Rfid_Id[i]=BufferA[i+1];
		LCD_ShowString(50, 180, "RFID_ID:");
		LCD_ShowString(114, 180, Rfid_Id);

		for(i=0;i<35;i++)
			Rfid_Value[i]=BufferA[i+12];
		LCD_ShowString(50, 200, "RFID_Value:");
		LCD_ShowString(138, 200, Rfid_Value);

	for(i=0;i<BufferCount;i++)
					 BufferA[i]=0x00;
			 BufferCount=0;
			 DBufferFlag=0;				
#endif				
		}
     IntMasterEnable();//使能全局中断
}

void UART2_Int_Config(void)
{
		//初始化和配置UART模块,PA0/PA1
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6);
    //UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);

	  //UART 中断配置
	  UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);
	  UARTIntRegister(UART2_BASE, UART2_Int_Handler);

    UARTIntEnable(UART2_BASE, UART_INT_RX);
    //使UART传输数据使能
    UARTIntEnable(UART2_BASE, UART_INT_TX);
}

void RFID_Test(void)
{
#if 1
	unsigned char i;	
	char Rfid_Id[9];
	char Rfid_Value[35];
	
	while(BufferCount<45);
		for(i=0;i<9;i++)
			Rfid_Id[i]=BufferA[i+1];
		//LCD_ShowString(50, 180, "RFID_ID:");
		//LCD_ShowString(114, 180, Rfid_Id);

		//for(i=0;i<35;i++)
		//	Rfid_Value[i]=BufferA[i+12];
		//LCD_ShowString(50, 200, "RFID_Value:");
		//LCD_ShowString(138, 200, Rfid_Value);

	//for(i=0;i<BufferCount;i++)
	///				 BufferA[i]=0x00;
	//		 BufferCount=0;
	//		 DBufferFlag=0;
	
#endif
}