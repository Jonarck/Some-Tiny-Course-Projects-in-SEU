/****************************************************************************************
** File Name  £ºLedBlink.c                                                             **
** Description:	Hanker LM4F232H5QC EVM board example for led test                      **
**                                                                                     **
**                                                                                     **
**                                                                                     **
=========================================================================================
** Author  :  Triton.Zhang smartztn@163.com                                            **
** Version :  v1.0                                                                     **
** History :                                                                           **
****************************************************************************************/
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "driverlib/rom.h"
#include "driverlib/fpu.H"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

#include "BoardHal/BoardHal.h"
#include "HalLed.h"

/****************************************************************************************
** File Name  £ºHAL_INIT_LED                                                           **
** Description:	                                                      			           **
** Arguments  :                                                                        **
** Output     :                                                                        **
** Return     :                                                                        **
=========================================================================================
** Author     :                                                                        **
** Version    :                                                                        **
** History    :                                                                        **
****************************************************************************************/
void HAL_INIT_LED(void)
{
	ROM_SysCtlPeripheralEnable(HAL_LED1_PERIPH);
	ROM_GPIODirModeSet(HAL_LED1_PORT, HAL_LED1_PIN, GPIO_DIR_MODE_OUT);
 	ROM_GPIOPadConfigSet(HAL_LED1_PORT, HAL_LED1_PIN, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

}
/****************************************************************************************
** File Name  £ºHAL_LED_ON                                                             **
** Description:	                                                      			           **
** Arguments  :                                                                        **
** Output     :                                                                        **
** Return     :                                                                        **
=========================================================================================
** Author     :                                                                        **
** Version    :                                                                        **
** History    :                                                                        **
****************************************************************************************/
void HAL_LED_ON(unsigned long nLed)
{
		if (nLed & LED_1)
		{
			ROM_GPIOPinWrite(HAL_LED1_PORT, HAL_LED1_PIN, HAL_LED1_PIN);
		}
}

/****************************************************************************************
** File Name  £ºHAL_LED_OFF                                                            **
** Description:	                                                      			           **
** Arguments  :                                                                        **
** Output     :                                                                        **
** Return     :                                                                        **
=========================================================================================
** Author     :                                                                        **
** Version    :                                                                        **
** History    :                                                                        **
****************************************************************************************/
void HAL_LED_OFF(unsigned long nLed)
{
		if (nLed & LED_1)
		{
			 ROM_GPIOPinWrite(HAL_LED1_PORT, HAL_LED1_PIN, ~HAL_LED1_PIN);
		}
}

/****************************************************************************************
** File Name  £ºHAL_LED_BLINK                                                          **
** Description:	                                                      			           **
** Arguments  :                                                                        **
** Output     :                                                                        **
** Return     :                                                                        **
=========================================================================================
** Author     :                                                                        **
** Version    :                                                                        **
** History    :                                                                        **
****************************************************************************************/
void HAL_LED_BLINK(unsigned long nLed)
{
		if (nLed & LED_1)
		{
				ROM_GPIOPinWrite(HAL_LED1_PORT, HAL_LED1_PIN, ~(ROM_GPIOPinRead(HAL_LED1_PORT, HAL_LED1_PIN)));
		}
}

