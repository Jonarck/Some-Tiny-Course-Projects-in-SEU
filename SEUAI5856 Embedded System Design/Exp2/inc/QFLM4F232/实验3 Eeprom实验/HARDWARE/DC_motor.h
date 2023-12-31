#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include "sys.h"
#include "lcd.h"
#include "PCF8574.h"


#define MOTOR_PE0_PERIPH        SYSCTL_PERIPH_GPIOG
#define MOTOR_PE0_BASE          GPIO_PORTE_BASE
#define MOTOR_PE0_PIN           GPIO_PIN_0

#define MOTOR_H         ROM_GPIOPinWrite(MOTOR_PE0_BASE, MOTOR_PE0_PIN, MOTOR_PE0_PIN)
#define MOTOR_L         ROM_GPIOPinWrite(MOTOR_PE0_BASE, MOTOR_PE0_PIN, 0)

void DC_Motor_delay(u32 tim);
void DC_Motor_GPIO_Init(void);
void DC_Motor_test(void);

#endif