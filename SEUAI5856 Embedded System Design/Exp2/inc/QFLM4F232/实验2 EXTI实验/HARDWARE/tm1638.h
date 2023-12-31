#ifndef __TM1638_H
#define __TM1638_H 	

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "sys.h"
#include "lcd.h"

#include "uart.h"

////¹Ü½Å¶¨Òå
//STB->PF2
#define STB_PERIPH        SYSCTL_PERIPH_GPIOF
#define STB_GPIO          GPIO_PORTF_BASE
#define STB_GPIO_PIN      0x00000004 //GPIO_Pin_2
#define STB_ONOFF(x)      ROM_GPIOPinWrite(STB_GPIO, STB_GPIO_PIN, x)
//CLK->PK2
#define CLK_PERIPH        SYSCTL_PERIPH_GPIOK
#define CLK_GPIO          GPIO_PORTK_BASE
#define CLK_GPIO_PIN      0x00000004 //GPIO_Pin_2
#define CLK_ONOFF(x)      ROM_GPIOPinWrite(CLK_GPIO, CLK_GPIO_PIN, x)

//DIO_PK0
#define DIO_PERIPH        SYSCTL_PERIPH_GPIOK
#define DIO_GPIO          GPIO_PORTK_BASE
#define DIO_GPIO_PIN      0x00000001 //GPIO_Pin_0
#define DIO_ONOFF(x)      ROM_GPIOPinWrite(DIO_GPIO, DIO_GPIO_PIN, x)



void TM1638_Write(unsigned char	DATA);
unsigned char TM1638_Read(void); 
void Write_COM(unsigned char cmd);
unsigned char Read_key(void);
void Write_DATA(unsigned char add,unsigned char DATA);
void Write_oneLED(unsigned char num,unsigned char flag);
void Init_TM1638(void);	
void TM1638_GPIO_Configuration(void);

#endif									   