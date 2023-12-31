#ifndef _STEP_MOTOR_H_
#define _STEP_MOTOR_H_

#include "sys.h"
#include "lcd.h"

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

/************步进电机控制管脚******************/
//PD12  PG2   PG4  PD11  OLD
//PF3   PF4   PE5  PE3   OLD
//PE6   PD12  PD6  PE5   OLD
//PH5   PH6   PH7  PH4	 NEW
//PE5->PH4
#define IN1_PERIPH        SYSCTL_PERIPH_GPIOH
#define IN1_GPIO          GPIO_PORTH_BASE
#define IN1_GPIO_PIN      0x00000010			//GPIO_Pin_4
#define IN1_ONOFF(x)      ROM_GPIOPinWrite(IN1_GPIO, IN1_GPIO_PIN, x)
//PE6->PH5
#define IN2_PERIPH        SYSCTL_PERIPH_GPIOH
#define IN2_GPIO          GPIO_PORTH_BASE
#define IN2_GPIO_PIN      0x00000020			//GPIO_Pin_5
#define IN2_ONOFF(x)      ROM_GPIOPinWrite(IN2_GPIO, IN2_GPIO_PIN, x)
//PD6->PH7
#define IN3_PERIPH        SYSCTL_PERIPH_GPIOH
#define IN3_GPIO          GPIO_PORTH_BASE
#define IN3_GPIO_PIN      0x00000080			//GPIO_Pin_7
#define IN3_ONOFF(x)      ROM_GPIOPinWrite(IN3_GPIO, IN3_GPIO_PIN, x)
//PD12->PH6
#define IN4_PERIPH        SYSCTL_PERIPH_GPIOH
#define IN4_GPIO          GPIO_PORTH_BASE
#define IN4_GPIO_PIN      0x00000040			//GPIO_Pin_6
#define IN4_ONOFF(x)      ROM_GPIOPinWrite(IN4_GPIO, IN4_GPIO_PIN, x)

void STEP_Motor_delay(u32 tim);
void STEP_Motor_GPIO_Init(void);
void BUJIN_port(u8 dat);
void STEP_Motor_test(void);

#endif