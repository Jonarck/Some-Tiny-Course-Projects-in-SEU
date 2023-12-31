#ifndef _MMA7455_H_
#define _MMA7455_H_
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include  <math.h>    //Keil library
#include "lcd.h"

#define   uchar unsigned char
#define   uint unsigned int	

#define	MMA7455_SlaveAddress	0x1D	//IIC写入时的地址 
#define MMA7455_IIC_READ  						((0x1D << 1) | 0x01)  //定义读指令
#define MMA7455_IIC_WRITE 						(0x1D << 1)  //定义写指令
//************************************
/*模拟IIC端口输出输入定义*/
#define SCL_PERIPH           SYSCTL_PERIPH_GPIOB
#define SCL_BASE             GPIO_PORTB_BASE
#define SCL_PIN              GPIO_PIN_2

#define SDA_PERIPH           SYSCTL_PERIPH_GPIOB
#define SDA_BASE             GPIO_PORTB_BASE
#define SDA_PIN              GPIO_PIN_3

#define SCL_H         ROM_GPIOPinWrite(SCL_BASE, SCL_PIN, SCL_PIN) 
#define SCL_L         ROM_GPIOPinWrite(SCL_BASE, SCL_PIN, 0)
   
#define SDA_H         ROM_GPIOPinWrite(SDA_BASE, SDA_PIN, SDA_PIN)
#define SDA_L         ROM_GPIOPinWrite(SDA_BASE, SDA_PIN, 0)

#define SCL_read      ROM_GPIOPinRead(SCL_BASE,SCL_PIN)//GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      ROM_GPIOPinRead(SDA_BASE,SDA_PIN)//GPIOB->IDR  & GPIO_Pin_7

void MMA7455_delay50us(void);
void MMA7455_data_init(void);
unsigned char MMA7455_self_test7455(void);
void MMA7455_sendxyz(void);
void MMA7455_Test(void);









#endif
