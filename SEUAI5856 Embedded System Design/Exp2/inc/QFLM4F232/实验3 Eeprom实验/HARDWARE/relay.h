#ifndef _RELAY_H_
#define _RELAY_H_
#include "sys.h"
#include "PCF8574.h"

void Relay_delay_tim(u32 tim);
void Relay_Gpio_Init(void);
void Relay_test(void);

#endif
