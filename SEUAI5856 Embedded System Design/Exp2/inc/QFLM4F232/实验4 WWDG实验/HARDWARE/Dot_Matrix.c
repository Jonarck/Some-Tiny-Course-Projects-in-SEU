
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include "Dot_Matrix.h"


// DIP_A: 	PB5   
// DIP_B: 	PK1		
// DIP_C: 	PE7		
// DIP_D: 	PB4 	
// DIP_G: 	PF1		
// DIP_DI:	PK3		
// DIP_DCLK:PE6 	
// DIP_LAT: PG6	
/************点阵显示接口******************/
#define D_PERIPH        SYSCTL_PERIPH_GPIOB
#define D_GPIO          GPIO_PORTB_BASE
#define D_GPIO_PIN      0x00000010  	//0x00000010		//GPIO_Pin_4
#define D_ONOFF(x)      ROM_GPIOPinWrite(D_GPIO, D_GPIO_PIN, x)

#define C_PERIPH        SYSCTL_PERIPH_GPIOE
#define C_GPIO          GPIO_PORTE_BASE
#define C_GPIO_PIN      0x00000080    //GPIO_Pin_7
#define C_ONOFF(x)      ROM_GPIOPinWrite(C_GPIO, C_GPIO_PIN, x)

#define B_PERIPH        SYSCTL_PERIPH_GPIOK
#define B_GPIO          GPIO_PORTK_BASE
#define B_GPIO_PIN      0x00000002    //GPIO_Pin_1
#define B_ONOFF(x)      ROM_GPIOPinWrite(B_GPIO, B_GPIO_PIN, x)

#define A_PERIPH        SYSCTL_PERIPH_GPIOB
#define A_GPIO          GPIO_PORTB_BASE
#define A_GPIO_PIN      0x00000020    //GPIO_Pin_5
#define A_ONOFF(x)      ROM_GPIOPinWrite(A_GPIO, A_GPIO_PIN, x)

#define G_PERIPH        SYSCTL_PERIPH_GPIOF
#define G_GPIO          GPIO_PORTF_BASE
#define G_GPIO_PIN      0x00000002    //GPIO_Pin_1
#define G_ONOFF(x)      ROM_GPIOPinWrite(G_GPIO, G_GPIO_PIN, x)

#define DI_PERIPH       SYSCTL_PERIPH_GPIOK
#define DI_GPIO         GPIO_PORTK_BASE
#define DI_GPIO_PIN     0x00000008    //GPIO_Pin_3
#define DI_ONOFF(x)     ROM_GPIOPinWrite(DI_GPIO, DI_GPIO_PIN, x)

#define DCLK_PERIPH     SYSCTL_PERIPH_GPIOE
#define DCLK_GPIO       GPIO_PORTE_BASE
#define DCLK_GPIO_PIN   0x00000040    //GPIO_Pin_6
#define DCLK_ONOFF(x)   ROM_GPIOPinWrite(DCLK_GPIO, DCLK_GPIO_PIN, x)

#define LAT_PERIPH      SYSCTL_PERIPH_GPIOG
#define LAT_GPIO        GPIO_PORTG_BASE
#define LAT_GPIO_PIN    0x00000040    //GPIO_Pin_6
#define LAT_ONOFF(x)    ROM_GPIOPinWrite(LAT_GPIO, LAT_GPIO_PIN, x)

unsigned char ziku_table[]={
0xFF,0xF7,0x80,0xF5,0xEB,0xF5,0xEB,0xC1,0x80,0xF5,0xAA,0xF6,0xAA,0xF7,0xAA,0x80,
0x8A,0xFF,0xBC,0xFF,0xBE,0xC1,0x80,0xDD,0xBE,0xDD,0xBE,0xDD,0x80,0xC1,0xBE,0xDD,/*"?",0*/
};

void Dot_Matric_Gpio_Init(void)
{
	// Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	
    ROM_GPIOPinTypeGPIOOutput(A_GPIO, 	A_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(B_GPIO, 	B_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(C_GPIO, 	C_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(D_GPIO, 	D_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(G_GPIO, 	G_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(DI_GPIO,  DI_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(DCLK_GPIO,DCLK_GPIO_PIN);
    ROM_GPIOPinTypeGPIOOutput(LAT_GPIO, LAT_GPIO_PIN);	
}

/******************************************************************************************
*函数名称：void ABCD_port(unsigned char dat)
*
*入口参数：dat
*
*出口参数：无
*
*功能说明：修改ABCD的值
*******************************************************************************************/
void ABCD_port(unsigned char dat)
{
	if((dat&0x01)==0x00)
		A_ONOFF(0);
	else A_ONOFF(A_GPIO_PIN);
	
	if((dat&0x02)==0x00)
		B_ONOFF(0);
	else B_ONOFF(B_GPIO_PIN);
	
	if((dat&0x04)==0x00)
		C_ONOFF(0);
	else C_ONOFF(C_GPIO_PIN);
	
	if((dat&0x08)==0x00)
		D_ONOFF(0);
	else D_ONOFF(D_GPIO_PIN);
}

void Dot_Matrix_test(void)
{
	unsigned char  i = 8,j = 0;
	unsigned char data_buff;
	unsigned char hanzi = 0;
	unsigned char shift = 1;	
	
	//Dot_Matric_Gpio_Init();
			
	for(shift=0;shift<16;shift++){	//一个字移位16位
			//HC595锁定输出,避免数据传输过程中，屏数据变化从而显示闪烁
			LAT_ONOFF(0);
			DCLK_ONOFF(0);
			//74HC595输出数据Hzk16
			data_buff = ziku_table[hanzi*32+shift*2+1];
		
			for(i=0;i<8;i++){
					if((data_buff&0x80)!=0) DI_ONOFF(DI_GPIO_PIN);//DA_in_port = 1;
					else DI_ONOFF(0);//DA_in_port = 0;
					DCLK_ONOFF(DCLK_GPIO_PIN);//CLK_port = 1;
					DCLK_ONOFF(0);//CLK_port = 0;
					data_buff <<= 1;
				}
				data_buff = ziku_table[hanzi*32+shift*2];
				
				for(i = 0; i < 8; i++){
					if((data_buff&0x01)!=0) 
						DI_ONOFF(DI_GPIO_PIN);//DA_in_port = 1;
					else 
						DI_ONOFF(0);//DA_in_port = 0;
					DCLK_ONOFF(DCLK_GPIO_PIN);//CLK_port = 1;
					DCLK_ONOFF(0);//CLK_port = 0;
					data_buff >>= 1;
				}
			//74HC138输出控制	 
			G_ONOFF(G_GPIO_PIN);//EN_port = 1;    //关屏显示，原理为使HC138输出全为1，从而三极管截止，点阵不显示
			ABCD_port(shift);//ABCD_port = (ABCD_port & 0x0f)|(han<<4);  //HC138译码输出
			//ABCD_port(han);//ABCD_port = (ABCD_port & 0x0f)|(han<<4);  //HC138译码输出
			LAT_ONOFF(LAT_GPIO_PIN);//Latch_port = 1; //允许HC595数据输出到Q1-Q8端口
			G_ONOFF(0);//EN_port = 0;    //HC138输出有效，打开显示
			LAT_ONOFF(0);//Latch_port = 0;	//锁定HC595数据输出
	}
}
	
