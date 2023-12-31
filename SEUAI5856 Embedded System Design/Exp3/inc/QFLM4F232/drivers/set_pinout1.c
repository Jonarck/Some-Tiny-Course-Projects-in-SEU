/************************************************************************************
** File  Name  : Touch.c                                                           **
** Description : MMC/SDC (in SPI mode) control module  (C)ChaN, 2007               **
**               This file was modified from a sample available from the FatFs Web **
**               site. It was modified to work with a Stellaris QF-LM4F232H5QC     **
**               evaluation board.                                                 **
**               This is part of revision 8034 of the EK-LM4F232 Firmware Package. **
** Notes       : Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros     **
**               are platform dependent.                                           **
=====================================================================================
** Author Name : Triton.zhang                                                      **
** Data        : 2011-10-20                                                        **
** Notes       : This module can be built in two ways.  If the                     **
**               label SIMPLE_PINOUT_SET is not defined, the PinoutSet() function  **
**               will attempt to read an I2C EEPROM to determine which daughter    **
**               board is attached to the development kit board and use information**
**               from that EEPROM to dynamically configure the EPI appropriately.  **
**               In this case, if no EEPROM is found, the EPI configuration will   **
**               default to that required to use the SDRAM daughter board which is **
**               included with the base development kit.                           **
**               If SIMPLE_PINOUT_SET is defined, however, all the dynamic         **
**               configuration code is replaced with a very simple function which  **
**               merely sets the pinout and EPI configuration statically.  This is **
**               a better representation of how a real-world application would     **
**               likely initialize the pinout and EPI timing and takes             **
**               significantly less code space than the dynamic, daughter-board    **
**               detecting version.  The example offered here sets the pinout and  **
**               EPI configuration appropriately for the Flash/SRAM/LCD or FPGA/   **
**               Camera/LCD daughter board depending upon another label definition.**
**               If EPI_CONFIG_FPGA is defined, the configuration for the FPGA     **
**               daughter board is set, otherwise the Flash/SRAM/LCD daughter board**
**               configuration is used.                                            **
************************************************************************************/
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"

#include "sysctl.h"
#include "i2c.h"
#include "gpio.h"
//#include "riverlib/epi.h"
#include "driverlib/debug.h"
#include "set_pinout.h"
//#include "camerafpga.h"

//===================================================================================
//===================================================================================

//===================================================================================
// A global variable indicating which daughter board, if any, is currently
// connected to the LM4F232H5QC development board.
//===================================================================================
tDaughterBoard g_eDaughterType;

////===================================================================================
//// The maximum number of GPIO ports.
////*****************************************************************************
//#define NUM_GPIO_PORTS 	11
//#define EPI_ENABLE		0
////===================================================================================
//// Base addresses of the GPIO ports that may contain EPI signals.  The index
//// into this array must correlate with the index in the ucPortIndex field of
//// tEPIPinInfo.
////===================================================================================
//const unsigned long g_pulGPIOBase[NUM_GPIO_PORTS] =
//{
//    GPIO_PORTA_BASE,
//    GPIO_PORTB_BASE,
//    GPIO_PORTC_BASE,
//    GPIO_PORTD_BASE,
//    GPIO_PORTE_BASE,
//    GPIO_PORTF_BASE,
//    GPIO_PORTG_BASE,
//    GPIO_PORTH_BASE,
//    GPIO_PORTJ_BASE,
//	GPIO_PORTK_BASE,
//	GPIO_PORTL_BASE
//};
//
//#if (EPI_ENABLE == 1)
////===================================================================================
//// Structure used to map an EPI signal to a GPIO port and pin on the target
//// part (LM4F232H5QC in this case).  Field ucPortIndex is the index into the
//// g_pulGPIOBase array containing the base address of the port.
////===================================================================================
//typedef struct
//{
//    unsigned char ucPortIndex;
//    unsigned char ucPctl;
//    unsigned char ucPin;
//}
//tEPIPinInfo;
//
////===================================================================================
//// The maximum number of EPI interface signals (EPI0Sxx).
////===================================================================================
//#define NUM_EPI_SIGNALS 32
//
////===================================================================================
//// The number of EPI clock periods for a write access with no wait states.
////===================================================================================
//#define EPI_WRITE_CYCLES 4
//
////===================================================================================
//// The number of EPI clock periods for a read access with no wait states.
////===================================================================================
//#define EPI_READ_CYCLES  4
//
////===================================================================================
//// The number of EPI clock periods added for each wait state.
////===================================================================================
//#define EPI_WS_CYCLES    2
//
////===================================================================================
//// This array holds the information necessary to map an EPI signal to a
//// particular GPIO port and pin on the target part (lm3s9b96 in this case) and
//// also the port control nibble required to enable that EPI signal.  The index
//// into the array is the EPI signal number.
////===================================================================================
//static const tEPIPinInfo g_psEPIPinInfo[NUM_EPI_SIGNALS] =
//{
//    {7, 8, 3},       // EPI0S00 on PH3
//    {7, 8, 2},       // EPI0S01 on PH2
//    {2, 8, 4},       // EPI0S02 on PC4
//    {2, 8, 5},       // EPI0S03 on PC5
//    {2, 8, 6},       // EPI0S04 on PC6
//    {2, 8, 7},       // EPI0S05 on PC7
//    {7, 8, 0},       // EPI0S06 on PH0
//    {7, 8, 1},       // EPI0S07 on PH1
//    {4, 8, 0},       // EPI0S08 on PE0
//    {4, 8, 1},       // EPI0S09 on PE1
//    {7, 8, 4},       // EPI0S10 on PH4
//    {7, 8, 5},       // EPI0S11 on PH5
//    {5, 8, 4},       // EPI0S12 on PF4
//    {6, 8, 0},       // EPI0S13 on PG0
//    {6, 8, 1},       // EPI0S14 on PG1
//    {5, 8, 5},       // EPI0S15 on PF5
//    {8, 8, 0},       // EPI0S16 on PJ0
//    {8, 8, 1},       // EPI0S17 on PJ1
//    {8, 8, 2},       // EPI0S18 on PJ2
//    {8, 8, 3},       // EPI0S19 on PJ3
//    {3, 8, 2},       // EPI0S20 on PD2
//    {3, 8, 3},       // EPI0S21 on PD3
//    {1, 8, 5},       // EPI0S22 on PB5
//    {1, 8, 4},       // EPI0S23 on PB4
//    {4, 8, 2},       // EPI0S24 on PE2
//    {4, 8, 3},       // EPI0S25 on PE3
//    {7, 8, 6},       // EPI0S26 on PH6
//    {7, 8, 7},       // EPI0S27 on PH7
//    {8, 8, 4},       // EPI0S28 on PJ4
//    {8, 8, 5},       // EPI0S29 on PJ5
//    {8, 8, 6},       // EPI0S30 on PJ6
//    {6, 9, 7}        // EPI0S31 on PG7
//};
//
////===================================================================================
//// Bit mask defining the EPI signals (EPI0Snn, for 0 <= n < 32) required for
//// the default configuration (in this case, we assume the SDRAM daughter board
//// is present).
////===================================================================================
//#define EPI_PINS_SDRAM 0xF00FFFFF
//
//#endif
//
//
//
///************************************************************************************
//** Function Name : PortControlSet                                                  **
//** Description   : Set the GPIO port control registers appropriately for the       **
//**                 hardware.                                                       **
//**                 This function determines the correct port control settings to   **
//**                 enable the basic peripheral signals for the qf-lm4f232h5qc on   **
//**                 their respective pins      								       **
//** Arguments     :                                                                 **
//** Return        :                                                                 **
//** Output        :                                                                 **
//=====================================================================================
//** Author        : Triton.zhang                                                    **
//** Data          : 2011-10-20                                                      **
//** Notes         :                                                                 **
//************************************************************************************/
//static void PortControlSet(void)
//{
//    unsigned long ulPctl[NUM_GPIO_PORTS];
//
//    // GPIO Port A pins
//    ulPctl[0] = 0;
//    // GPIO Port B pins
//    ulPctl[1] = 0;
//
//    // GPIO Port C pins
//    ulPctl[2] = 0;
//
//    // GPIO Port D pins.
//    ulPctl[3] = 0;
//
//    // GPIO Port E pins
//    ulPctl[4] = 0;
//
//    // GPIO Port F pins
//    ulPctl[5] = 0;
//
//    // GPIO Port G pins
//    ulPctl[6] = 0;
//
//    // GPIO Port H pins
//    ulPctl[7] = 0;
//
//    // GPIO Port J pins
//    ulPctl[8] = 0;
//
//   	// GPIO Port K pins
//    ulPctl[8] = 0;
//
//	// GPIO Port L pins
//    ulPctl[8] = 0;
//
//    // Now that we have determined the required configuration, set the actual
//    // port control registers.
//    HWREG(GPIO_PORTA_BASE + GPIO_O_PCTL) = ulPctl[0];
//    HWREG(GPIO_PORTB_BASE + GPIO_O_PCTL) = ulPctl[1];
//    HWREG(GPIO_PORTC_BASE + GPIO_O_PCTL) = ulPctl[2];
//    HWREG(GPIO_PORTD_BASE + GPIO_O_PCTL) = ulPctl[3];
//    HWREG(GPIO_PORTE_BASE + GPIO_O_PCTL) = ulPctl[4];
//    HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) = ulPctl[5];
//    HWREG(GPIO_PORTG_BASE + GPIO_O_PCTL) = ulPctl[6];
//    HWREG(GPIO_PORTH_BASE + GPIO_O_PCTL) = ulPctl[7];
//    HWREG(GPIO_PORTJ_BASE + GPIO_O_PCTL) = ulPctl[8];
//    HWREG(GPIO_PORTK_BASE + GPIO_O_PCTL) = ulPctl[9];
//    HWREG(GPIO_PORTL_BASE + GPIO_O_PCTL) = ulPctl[10];
//
//}
//
///************************************************************************************
//** Function Name : PinoutSet                                                       **
//** Description   : Configures the QF-LM4F232H5QC device pinout for the development **
//**                 board.                                                          **
//**                 This function configures each pin of the LM4F232H5QC device to  **
//**                 route the appropriate peripheral signal as required by the      **
//**                 design of the  qf-lm4f232h5qc development board.                **
//** Arguments     :                                                                 **
//** Return        :                                                                 **
//** Output        :                                                                 **
//=====================================================================================
//** Author        : Triton.zhang                                                    **
//** Data          : 2011-10-20                                                      **
//** Notes         :                                                                 **
//************************************************************************************/
//void PinoutSet(void)
//{
//    // Enable all GPIO banks.
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
//
//    PortControlSet();
//   
//}
