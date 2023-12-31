#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "PCF8574.h"
#include "lcd.h"
//#include "string.h"		//include : strstr()
//#include "usart.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"


#define KEY_PG0_PERIPH        SYSCTL_PERIPH_GPIOG
#define KEY_PG0_BASE          GPIO_PORTG_BASE
#define KEY_PG0_PIN           GPIO_PIN_0


//extern uint8_t gsm_ch[GSM_length];

// 
//#define KEY0 PEin(4)  //PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
 
//#define KEY4  ROM_GPIOPinRead(GPIOE,GPIO_Pin_4)//????0
//#define KEY2  ROM_GPIOPinRead(GPIOE,GPIO_Pin_1)//????1
#define KEY1  ROM_GPIOPinRead(GPIO_PORTG_BASE,KEY_PG0_PIN)//
//#define KEY3  ROM_GPIOPinRead(GPIOA,GPIO_Pin_0)//????3(WK_UP) 

#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void Delay5ms(void);
void KEY_Init(void);//IO???	
void KEY_Test(void);
#endif
