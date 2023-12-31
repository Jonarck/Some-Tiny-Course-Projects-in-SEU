#ifndef __DHT11_H
#define __DHT11_H 	

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include "lcd.h"
#include "PCF8574.h"
#include "sys.h"
extern unsigned char sbuf;
extern  unsigned char tdata[4];

#define DHT11_PERIPH           SYSCTL_PERIPH_GPIOG
#define DHT11_BASE             GPIO_PORTG_BASE
#define DHT11_PIN              GPIO_PIN_7

#define DHT11_H         ROM_GPIOPinWrite(DHT11_BASE, DHT11_PIN, DHT11_PIN)
#define DHT11_L         ROM_GPIOPinWrite(DHT11_BASE, DHT11_PIN, 0)


#define READ_DEHT11            ROM_GPIOPinRead(DHT11_BASE, DHT11_PIN)

unsigned char start_DHT11(void);
void DTH11_IN();
void DTH11_OUT(void); //配置TM1638DIO引脚为推挽输出
unsigned long DHT11_Get_Data(void);//获取数据
void DTH11_RH(void);
void Send_DTH11(void);
void  DTH11_COM(void);
void DHT11_Test(void);
void GAS_Test(void);
void HUMAN_Test(void);
#endif									   





























