/************************************************************************************
** File  Name  : board_hal_sys.c                                                   **
** Description : 						                                           **
**               This is part of revision 8034 of the QF-LM4F232 Firmware Package. **
=====================================================================================
** Author Name : smartztn                                                          **
** Data        : 2011-10-20                                                        **
** Notes       :                                                                   **
************************************************************************************/

#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"

#include "fpu.H"
#include "sysctl.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"
#include "rom.h"

#include "grlib.h"

#include "board_hal_sys.h"



/************************************************************************************
** Function Name : hal_clock_init                                                  **
** Description   :                                                                 **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : smartztn                                                        **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void hal_clock_init(void)
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

}

/************************************************************************************
** Function Name : hal_sys_delay                                                   **
** Description   : Delay for the specified number of seconds.  Depending upon the  **
**                 current SysTick value, the delay will be between N-1 and N      **
**                 seconds (i.e. N-1 full seconds are guaranteed, along with the   **
**                 remainder of the current second).                               **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : smartztn                                                        **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void hal_sys_delay(unsigned long ulSeconds)
{
 	// Loop while there are more seconds to wait.
    while(ulSeconds--)
    {
        // Wait until the SysTick value is less than 1000.
        while(ROM_SysTickValueGet() > 1000)
        {
        }

        // Wait until the SysTick value is greater than 1000.
        while(ROM_SysTickValueGet() < 1000)
        {
        }
    }
}


