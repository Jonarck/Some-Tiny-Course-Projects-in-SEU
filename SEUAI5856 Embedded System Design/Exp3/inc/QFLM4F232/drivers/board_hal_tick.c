/************************************************************************************
** File  Name  : board_hal_tick.c                                                  **
** Description : 						                                           **
**               This is part of revision 8034 of the QF-LM4F232 Firmware Package. **
=====================================================================================
** Author Name : Triton.zhang                                                      **
** Data        : 2011-10-20                                                        **
** Notes       :                                                                   **
************************************************************************************/

#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"

#include <LM4Fxxxx.H>
#include "fpu.H"
#include "sysctl.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"
#include "rom.h"

#include "grlib.h"

#include "board_hal_tick.h"



//===================================================================================
// 系统全局变量，记录系统tick时钟
//===================================================================================
volatile unsigned long g_ulSysTickCount = 0;

/************************************************************************************
** Function Name : usb_uart_prime_transmit                                         **
** Description   : Interrupt handler for the system tick counter.       		   **	  
** Arguments     :                                                                 **
** Return        : 														           **
** Output        :                                                                 **
=====================================================================================
** Author        : smartztn@163.com                                                **
** Data          : 2012-02-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void sys_tick_int_handler(void)
{
    // Update our system time.
    g_ulSysTickCount++;
}

/************************************************************************************
** Function Name : sys_enalbe_tick                                                 **
** Description   : Interrupt handler for the system tick counter.       		   **	  
** Arguments     :                                                                 **
** Return        : 														           **
** Output        :                                                                 **
=====================================================================================
** Author        : smartztn@163.com                                                **
** Data          : 2012-02-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void hal_sys_tick_enalbe(void)
{
    // Enable the system tick.
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
}

