/****************************************************************************************
** File Name  £ºHalLcd.c                                                               **
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

#include "drivers/BoardHal.h"
#include "drivers/HalLcd.h"

/************************************************************************************
** Function Name : HAL_LCD_WRITE_DATA                                              **
** Description   : Writes a data word to the lcd module                            **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void HAL_LCD_WRITE_DATA(unsigned short usData)
{
    LCD_SET_RS(HAL_LCD_RS_PIN);

    // Write the data to the data bus.
	  LCD_SET_DATAH((unsigned char)(usData >> 8));
	  LCD_SET_DATAL((unsigned char)usData);

    // Assert the write enable signal.
	  LCD_SET_WR(0);

    // Deassert the write enable signal.
    LCD_SET_WR(HAL_LCD_WR_PIN);
}

/************************************************************************************
** Function Name : HAL_LCD_READ_DATA                                               **
** Description   : Writes a data word to the lcd module                            **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
unsigned short HAL_LCD_READ_DATA(void)
{
    unsigned short usData;

    // Make the data bus be an input.
		ROM_GPIODirModeSet(HAL_LCD_DATAH_PORT, HAL_LCD_DATAH_PINS, GPIO_DIR_MODE_IN);
		ROM_GPIODirModeSet(HAL_LCD_DATAL_PORT, HAL_LCD_DATAL_PINS, GPIO_DIR_MODE_IN);

		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;

		ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, 0x80);
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

		// Assert the read signal.  This is done multiple times (though only the
		// first has an affect on the pin) in order to meet the timing requirements
		// of the ILI932x.
		LCD_SET_RD(0);
		LCD_SET_RD(0);
		LCD_SET_RD(0);

    // Read the data from the data bus.
    //usData = LCD_READ_DATA_16BIT();
	  usData = HWREG(HAL_LCD_DATAH_PORT + GPIO_O_DATA + (0xff << 2)) << 8;
    usData |= HWREG(HAL_LCD_DATAL_PORT + GPIO_O_DATA + (0xff << 2));

    // Deassert the read signal.
    LCD_SET_RD(HAL_LCD_RD_PIN)
    LCD_SET_RD(HAL_LCD_RD_PIN)
 
    // Change the data bus back to an output.
		ROM_GPIODirModeSet(HAL_LCD_DATAH_PORT, HAL_LCD_DATAH_PINS, GPIO_DIR_MODE_OUT);
		ROM_GPIODirModeSet(HAL_LCD_DATAL_PORT, HAL_LCD_DATAL_PINS, GPIO_DIR_MODE_OUT);
    
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)   = 0x80;
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)   = 0x00;
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

    // Return the data that was read.
    return(usData);
}
/************************************************************************************
** Function Name : HAL_LCD_WRITE_COMMAND                                           **
** Description   : Writes a command to the lcd module                              **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void HAL_LCD_WRITE_COMMAND(unsigned char ucData)
{
    // Write the command to the data bus.
	  LCD_SET_DATAH(0);
	  LCD_SET_DATAL(ucData);
	
    // Set the RS signal low, indicating a command.
	  LCD_SET_RS(0);

    // Assert the write enable signal.
	  LCD_SET_WR(0);
 
    // Deassert the write enable signal.
    LCD_SET_WR(HAL_LCD_WR_PIN);

    // Set the RS signal high, indicating that following writes are data.
	  LCD_SET_RS(HAL_LCD_RS_PIN);

}

/************************************************************************************
** Function Name : HAL_LCD_INIT_GPIO                                               **
** Description   : Writes a command to the lcd module                              **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void HAL_LCD_INIT_GPIO(void)
{
	  //unsigned long ulClockMS;
	  //ulClockMS = ROM_SysCtlClockGet() / (3 * 1000);
	
	  // Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(HAL_LCD_CTL_PERIPH);
    ROM_SysCtlPeripheralEnable(HAL_LCD_BL_PERIPH);
	  ROM_SysCtlPeripheralEnable(HAL_LCD_RST_PERIPH);
	
    // Configure the pins that connect to the LCD as GPIO outputs.
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xFF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0xFF);
    ROM_GPIOPinTypeGPIOOutput(HAL_LCD_CTL_PORT, HAL_LCD_RS_PIN | HAL_LCD_RD_PIN | HAL_LCD_WR_PIN | HAL_LCD_CS_PIN);
	  ROM_GPIOPinTypeGPIOOutput(HAL_LCD_BL_PORT, HAL_LCD_BL_PIN);
    ROM_GPIOPinTypeGPIOOutput(HAL_LCD_RST_PORT, HAL_LCD_RST_PIN);

		  
		LCD_SET_BL(0);	

	  // PD.7 default as NMI Port
	  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	  HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
    
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
 	
	  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	  HWREG(GPIO_PORTD_BASE + GPIO_O_CR)   = 0x00;
	  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	 
	  LCD_SET_DATAH(0xFF);
		LCD_SET_DATAL(0xFF);
		
	  LCD_SET_DATAH(0x0);
		LCD_SET_DATAL(0x0);
		
		LCD_SET_DATAH(0xFF);
		LCD_SET_DATAL(0xFF);
		
}


