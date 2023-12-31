#ifndef _BMP085_H_
#define _BMP085_H_

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include "sys.h"
#include "lcd.h"

#define	BMP085_SlaveAddress	0xEE	//IIC写入时的地址字节数据，+1为读取

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

typedef enum { FALSE = 0, TRUE } BOOL;
#define bool BOOL

void I2C_GPIO_Config(void);
void InitBMP085(void);
void bmp085Convert();

int GetData(unsigned char REG_Address);
void DATA_Diplay();

void I2C_Gpio_Init(void);


void I2C_Start();
void I2C_Stop();
void I2C_SendACK(char ack);
char I2C_RecvACK();
void I2C_SendByte(u8 dat);
u8 I2C_RecvByte(void);
void Single_WriteI2C(u8 SlaveAddress, u8 REG_Address,u8 REG_data);
u8 Single_ReadI2C(u8 SlaveAddress, u8 REG_Address);
short Multiple_read(u8 SlaveAddress, u8 ST_Address);
void Acc_Gas_Test(void);
void BMP085_Test(void);


#endif