#ifndef _PCF8574_IIC_H_
#define _PCF8574_IIC_H_
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "sys.h"
#include  <math.h>    //Keil library


//A0 A1 A2
//L  L  L
#define PCF8574T_A 0x20										
#define PCF8574T_A_W (PCF8574T_A << 1)
#define PCF8574T_A_R ((PCF8574T_A << 1) | 0x01)
//A0 A1 A2
//L  L  H
#define PCF8574T_B 0x21										
#define PCF8574T_B_W (PCF8574T_B << 1)
#define PCF8574T_B_R ((PCF8574T_B << 1) | 0x01)
//H  H  L
#define PCF8574T_C 0x23										
#define PCF8574T_C_W (PCF8574T_C << 1)
#define PCF8574T_C_R ((PCF8574T_C << 1) | 0x01)
//H  H  H
#define PCF8574T_E 0x27										
#define PCF8574T_E_W (PCF8574T_D << 1)
#define PCF8574T_E_R ((PCF8574T_D << 1) | 0x01)

#define	PCF8574_SlaveAddress	0x27	//IIC写入时的地址 
#define IIC_READ  0x27      				//定义读指令
#define IIC_WRITE 0x27      				//定义写指令
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

void PCF8574_I2C_GPIO_Config(void);
void PCF8574_i2c_SDA_input(void);
void PCF8574_i2c_SDA_output(void);
void PCF8574_I2C_delay(void);
void PCF8574_delay5ms(void);
void PCF8574_delay50us(void);
void PCF8574_I2C_Start(void);
void PCF8574_I2C_Stop(void);
void PCF8574_I2C_SendACK(char ack);
char PCF8574_I2C_RecvACK(void);
void PCF8574_I2C_SendByte(uchar dat);
uchar PCF8574_I2C_RecvByte(void);
void PCF8574_Single_WriteI2C(uchar REG_Address,uchar REG_data);
uchar PCF8574_Single_ReadI2C(uchar REG_Address);


#endif

