/************************************************************************************
** File  Name  : S95196x240x320x16_ILI9320.c                                       **
** Description : Prototypes for the S95196 display driver with an ILI9320          **
**               controller.                                                       **
**               This is part of revision 8034 of the EK-LM4F232 Firmware Package. **
=====================================================================================
** Author Name : Triton.zhang                                                      **
** Data        : 2011-10-20                                                        **
** Email       : smartztn@163.com                                                  **
** Notes       :                                                                   **
************************************************************************************/
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "grlib/grlib.h"
#include "drivers/S95196x240x320x16_ILI9320.h"

//===================================================================================
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide, and the flex connector is on 
//              the bottom of the display.  This is selected by defining PORTRAIT.
// * Landscape - The screen is wider than it is tall, and the flex connector is on the 
//              right side of the display.  This is selected by defining LANDSCAPE.
// * Portrait flip - The screen is taller than it is wide, and the flex connector is 
//              on the top of the display.  This is selected by defining PORTRAIT_FLIP.
// * Landscape flip - The screen is wider than it is tall, and the flex connector is on 
//              the left side of the display.  This is selected by defining LANDSCAPE_FLIP.
// These can also be imagined in terms of screen rotation; if portrait mode is 0 degrees of 
// screen rotation, landscape is 90 degrees of counter-clockwise rotation, portrait flip 
// is 180 degrees of rotation, and landscape flip is270 degress of counter-clockwise rotation.
// If no screen orientation is selected, portrait mode will be used.
//===================================================================================
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define LANDSCAPE_FLIP
#endif
unsigned short g_usController;
/************************************************************************************
** Function Name : WriteData                                                       **
** Description   : Writes a data word to the ILI932x.                              **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void LCD_WriteData(unsigned short usData)
{
	LCD_SET_RS(LCD_RS_PIN);

    // Write the data to the data bus.
    //LCD_WRITE_DATA_16BIT(usData);
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (usData >> 8);   
    HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (usData);   

    // Assert the write enable signal.
	LCD_SET_WR(0);
	LCD_SET_WR(0);
	LCD_SET_WR(0);

    // Deassert the write enable signal.
    LCD_SET_WR(LCD_WR_PIN);
}

/************************************************************************************
** Function Name : ReadData                                                        **
** Description   : Reads a data word from the ILI932x.                             **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned short LCD_ReadData(void)
{
    unsigned short usData;

    // Make the data bus be an input.
	ROM_GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_IN);
	ROM_GPIODirModeSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_DIR_MODE_IN);

	HWREG(GPIO_PORTD_BASE +GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, 0x80);
 	HWREG(GPIO_PORTD_BASE +GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
 	HWREG(GPIO_PORTD_BASE +GPIO_O_LOCK) = 0;

    // Assert the read signal.  This is done multiple times (though only the
    // first has an affect on the pin) in order to meet the timing requirements
    // of the ILI932x.
	LCD_SET_RD(0);
	LCD_SET_RD(0);
	LCD_SET_RD(0);

    // Read the data from the data bus.
    //usData = LCD_READ_DATA_16BIT();
	usData = HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) << 8;
    usData |= HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (0xff << 2));

    // Deassert the read signal.
    LCD_SET_RD(LCD_RD_PIN)
    LCD_SET_RD(LCD_RD_PIN)
 
    // Change the data bus back to an output.
	ROM_GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_OUT);
	ROM_GPIODirModeSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_DIR_MODE_OUT);
    
	HWREG(GPIO_PORTD_BASE +GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
 	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
	HWREG(GPIO_PORTD_BASE +GPIO_O_LOCK) = 0;

    // Return the data that was read.
    return(usData);
}

/************************************************************************************
** Function Name : WriteCommand                                                    **
** Description   : Writes a command to the ILI932x.                                **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void LCD_WriteCommand(unsigned char ucData)
{
    // Write the command to the data bus.
	//LCD_WRITE_DATA_16BIT(ucData);
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucData >> 8);   
    HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (ucData);   

    // Set the RS signal low, indicating a command.
	LCD_SET_RS(0);

    // Assert the write enable signal.
	LCD_SET_WR(0);
 	LCD_SET_WR(0);
	LCD_SET_WR(0);   
    // Deassert the write enable signal.
    LCD_SET_WR(LCD_WR_PIN);

    // Set the RS signal high, indicating that following writes are data.
	LCD_SET_RS(LCD_RS_PIN);

}

/************************************************************************************
** Function Name : LCD_ReadRegister                                                **
** Description   : Read the value of a register from the ILI932x display controller**
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned short LCD_ReadRegister(unsigned char ucIndex)
{
    LCD_WriteCommand(ucIndex);
    return(LCD_ReadData());
}

/************************************************************************************
** Function Name : WriteRegister                                                   **
** Description   : Write a particular controller register with a value             **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void	LCD_WriteRegister(unsigned char ucIndex, unsigned short usValue)
{
    LCD_WriteCommand(ucIndex);
    LCD_WriteData(usValue);
}

/************************************************************************************
** Function Name : LCD_InitGpio                                                    **
** Description   :                                                                 **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void LCD_InitGpio(void)
{
	// Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    // Configure the pins that connect to the LCD as GPIO outputs.
    ROM_GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, LCD_DATAH_PINS);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LCD_RD_PIN | LCD_WR_PIN);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, LCD_LEDK_PIN);
    ROM_GPIOPinTypeGPIOOutput(LCD_DATAL_BASE, LCD_DATAL_PINS);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, LCD_CS_PIN | LCD_RS_PIN);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LCD_RST_PIN);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, LCD_LEDK_PIN);

	// PD.7 default as NMI Port
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
    
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
 	
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320Init                                   **
** Description   : Initializes the display driver.                                 **
** Arguments     : This function initializes the ILI9320, ILI9325 or ILI9328       **
**                 display controller on the panel, preparing it to display data.  **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void S95196x240x320x16_ILI9320Init(void)
{
    unsigned long ulClockMS, ulCount;
    unsigned short usController;

    // Get the current processor clock frequency.
    ulClockMS = SysCtlClockGet() / (3 * 1000);
	
	//===========================================================
    LCD_InitGpio();
	
	//===========================================================
    // Set the LCD control pins to their default values.  This also asserts the
    // LCD reset signal.
	ROM_GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0x00);						    // LCD_CS = 0
    ROM_GPIOPinWrite(LCD_DATAH_BASE, LCD_DATAH_PINS, 0x00);						// LCD_DATA_H = 0
    ROM_GPIOPinWrite(LCD_DATAL_BASE, LCD_DATAL_PINS, 0x00);						// LCD_DATA_L = 0

    ROM_GPIOPinWrite(LCD_RS_BASE, LCD_RS_PIN, LCD_RS_PIN);						// LCD_RS = 0
    ROM_GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);						// LCD_RD = 0
    ROM_GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);						// LCD_WR = 0
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0);						// LCD_LED_K1 = 1

	// Reset LCD
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);						        // LCD_RST = 0
	ROM_SysCtlDelay(5 * ulClockMS);
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0);						        // LCD_RST = 0
	
	// Delay for 20ms.
    ROM_SysCtlDelay(10 * ulClockMS);
	ROM_SysCtlDelay(10 * ulClockMS);
	
    // Deassert the LCD reset signal.
    ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);

    // Delay for 50ms while the LCD comes out of reset.
    ROM_SysCtlDelay(50 * ulClockMS);

    // Delay for 10ms while the oscillator stabilizes.
    ROM_SysCtlDelay(10 * ulClockMS);

    // Determine which version of the display controller we are using.
    usController = LCD_ReadRegister(0x00);

    // Internal timing configuration (common to both ILI9320, ILI9325 andILI9328)
    LCD_WriteRegister(0xE3, 0x3008);

	if (usController != 0x9320)
	{
	    // Set internal timing registers on the ILI9325/8 controller
       	LCD_WriteRegister(0xE7, 0x0012);
        LCD_WriteRegister(0xEF, 0x1231);
	}
    else
    {
        // Enable the internal oscillator (ILI9320 only)
 		LCD_WriteRegister(0x00, 0x0001);
    }

    // Basic interface configuration (common to all controllers).
    LCD_WriteRegister(0x01, 0x0100); // set SS and SM bit
    LCD_WriteRegister(0x02, 0x0700); // set 1 line inversion
    LCD_WriteRegister(0x03, 0x0030); // set GRAM write direction and BGR=0.
    LCD_WriteRegister(0x04, 0x0000); // Resize register
    LCD_WriteRegister(0x08, 0x0202); // set the back porch and front porch
    LCD_WriteRegister(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WriteRegister(0x0A, 0x0000); // FMARK function
    LCD_WriteRegister(0x0C, 0x0001); // RGB interface setting
    LCD_WriteRegister(0x0D, 0x0000); // Frame marker Position
    LCD_WriteRegister(0x0F, 0x0000); // RGB interface polarity

    // Power On sequence as provided by display manufacturer.
    LCD_WriteRegister(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteRegister(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WriteRegister(0x12, 0x0000); // VREG1OUT voltage
    LCD_WriteRegister(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
	
    ROM_SysCtlDelay(200 * ulClockMS);
    if (usController != 0x9320)
    {
        // Power on sequence for the ILI9325/8.
        LCD_WriteRegister(0x10, 0x1690);
        LCD_WriteRegister(0x11, 0x0227);

        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x12, 0x001A); // External Reference Voltage = VCI
        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x13, 0x1800);  // Set VDV[4:0] for VCOM amplitude
        LCD_WriteRegister(0x29, 0x002A);  // Set VCM[5:0] for VCOMH
        LCD_WriteRegister(0x2B, 0x000D);

        ROM_SysCtlDelay(50 * ulClockMS);
    }
    else
    {
        // Power on sequence for the ILI9320.
        LCD_WriteRegister(0x10, 0x17B0);
        LCD_WriteRegister(0x11, 0x0137);

        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x12, 0x013C);

        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x13, 0x1900);
        LCD_WriteRegister(0x29, 0x001A);

        ROM_SysCtlDelay(50 * ulClockMS);
    }

    // GRAM horizontal and vertical addresses
    LCD_WriteRegister(0x20, 0x0000);  // GRAM horizontal address
    LCD_WriteRegister(0x21, 0x0000);  // GRAM Vertical address

    // Adjust the Gamma Curve
    LCD_WriteRegister(0x30, (usController != 0x9320) ? 0x0007 : 0x0002);
    LCD_WriteRegister(0x31, (usController != 0x9320) ? 0x0605 : 0x0607);
    LCD_WriteRegister(0x32, (usController != 0x9320) ? 0x0106 : 0x0504);
    LCD_WriteRegister(0x35, 0x0206);
    LCD_WriteRegister(0x36, (usController != 0x9320) ? 0x0808: 0x0504);
    LCD_WriteRegister(0x37, (usController != 0x9320) ? 0x0007: 0x0606);
    LCD_WriteRegister(0x38, (usController != 0x9320) ? 0x0201: 0x0105);
    LCD_WriteRegister(0x39, 0x0007);
    LCD_WriteRegister(0x3C, (usController != 0x9320) ? 0x0602 : 0x0700);
    LCD_WriteRegister(0x3D, (usController != 0x9320) ? 0x0808 : 0x0700);
  
    // Set GRAM area
    LCD_WriteRegister(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_WriteRegister(0x51, 0x00EF); // Horizontal GRAM End Address
    LCD_WriteRegister(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_WriteRegister(0x53, 0x013F); // Vertical GRAM Start Address

    // Driver output control 2, base image display control and vertical scroll control.
    LCD_WriteRegister(0x60, (usController != 0x9320) ? 0xA700 : 0x2700);
	LCD_WriteRegister(0x61, 0x0001); // NDL,VLE, REV
    LCD_WriteRegister(0x6A, 0x0000); // set scrolling line

    // Partial Display Control
    LCD_WriteRegister(0x80, 0x0000);
    LCD_WriteRegister(0x81, 0x0000);
    LCD_WriteRegister(0x82, 0x0000);
    LCD_WriteRegister(0x83, 0x0000);
    LCD_WriteRegister(0x84, 0x0000);
    LCD_WriteRegister(0x85, 0x0000);

    // Panel Control
    LCD_WriteRegister(0x90, 0x0010);
    LCD_WriteRegister(0x92, 0x0000);
	LCD_WriteRegister(0x93, 0x0003);
    LCD_WriteRegister(0x95, 0x0110);
    LCD_WriteRegister(0x97, 0x0000);
    LCD_WriteRegister(0x98, 0x0000);

    // Clear the contents of the display buffer.
    LCD_WriteCommand(0x22);
    for (ulCount = 0; ulCount < (320 * 240); ulCount++)
    {
        LCD_WriteData(0x0000);
    }

    // Enable the image display.
    LCD_WriteRegister(0x07, 0x0133);

    // Delay for 20ms, which is equivalent to two frames.
    ROM_SysCtlDelay(20 * ulClockMS);
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320ControllerIdGet                        **
** Description   : Determines whether an ILI9320, ILI9325 or ILI9328 controller is **
**                 present.                                                        **
**                 This function queries the ID of the display controller in use   **
**                 and returns it to the caller.  This driver supports both ILI9320**
**                 , ILI9325 and ILI9328. These are very similar but the sense of  **
**                 the long display axis is reversed in the S95196 KWH028Q02-F03   **
**                 using an ILI9320 relative to the other two supported displays   **
**                 and this information is needed by the touchscreen driver to     **
**                 provide correct touch coordinate information.                   **
** Arguments     : This function initializes the ILI9320, ILI9325 or ILI9328       **
**                 display controller on the panel, preparing it to display data.  **
** Return        : Returns 0x9320 if an ILI9320 controller is in use, 0x9325 if an **
**                 ILI9325 is present or 0x9328 if an ILI9328 is detected.         **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
unsigned short S95196x240x320x16_ILI9320ControllerIdGet(void)
{
    unsigned short usController;

    // Determine which version of the display controller we are using.
    usController = LCD_ReadRegister(0x00);

    return (usController);
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320Init                                   **
** Description   : Turns on the backlight.                                         **
**				   This function turns on the backlight on the display.            **
** Arguments     :                                                                 **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void S95196x240x320x16_ILI9320BacklightOn(void)
{
    // Deassert the signal that turns on the backlight.

	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0);						// LCD_LED_K1 = 1


}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320BacklightOff                           **
** Description   : Turns off the backlight.                                        **
**				   This function turns off the backlight on the display.           **
** Arguments     :                                                                 **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void S95196x240x320x16_ILI9320BacklightOff(void)
{
    // Deassert the signal that turns off the backlight.
   	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, LCD_LEDK_PIN);	// LCD_LED_K1 = 1

}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320PixelDraw                              **
** Description   : Draws a pixel on the screen.                                    **
**				   This function sets the given pixel to a particular color.  The  **
**                 coordinates of the pixel are assumed to be within the extents of**
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the pixel.                          **
**				   lY - is the Y coordinate of the pixel.		                   **
**                 ulValue - is the color of the pixel.							   **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320PixelDraw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
    // Set the X address of the display cursor.
    LCD_WriteCommand(0x20);
#ifdef PORTRAIT
    LCD_WriteData(lX);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(lY);
#endif

    // Set the Y address of the display cursor.
    LCD_WriteCommand(0x21);
#ifdef PORTRAIT
    LCD_WriteData(lY);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the pixel value.
    LCD_WriteCommand(0x22);
    LCD_WriteData(ulValue);
}


/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320PixelDrawMultiple                      **
** Description   : Draws a horizontal sequence of pixels on the screen.            **
**				   This function draws a horizontal sequence of pixels on the      **
**                 screen, using the supplied palette.  For 1 bit per pixel format,**
**                 the palette contains pre-translated colors; for 4 and 8 bit per **
**                 pixel formats, the palette contains 24-bit RGB values that must **
**                 be translated before being written to the display.              **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the pixel.                          **
**				   lY - is the Y coordinate of the pixel.		                   **
**                 lX0 - is sub-pixel offset within the pixel data, which is valid **
**                       for 1 or 4 bit per pixel formats.					       **
**                 lCount - is the number of pixels to draw.                       **
**                 lBPP - is the number of bits per pixel; must be 1, 4, or 8.     **
**                 pucData - is a pointer to the pixel data.  For 1 and 4 bit per  **
**                           pixel formats, the most significant bit(s) represent  **
**                           the left-most pixel.                                  **
**                 pucPalette - is a pointer to the palette used to draw the pixels**
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320PixelDrawMultiple(void *pvDisplayData, long lX,
                                           long lY, long lX0, long lCount,
                                           long lBPP,
                                           const unsigned char *pucData,
                                           const unsigned char *pucPalette)
{
    unsigned long ulByte;

    // Set the cursor increment to left to right, followed by top to bottom.
    LCD_WriteCommand(0x03);
#ifdef PORTRAIT
    LCD_WriteData(0x0030);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(0x0018);
#endif

    // Set the starting X address of the display cursor.
    LCD_WriteCommand(0x20);
#ifdef PORTRAIT
    LCD_WriteData(lX);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(lY);
#endif

    // Set the Y address of the display cursor.
    LCD_WriteCommand(0x21);
#ifdef PORTRAIT
    LCD_WriteData(lY);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the data RAM write command.
    LCD_WriteCommand(0x22);

    // Determine how to interpret the pixel data based on the number of bits per pixel.
    switch (lBPP)
    {
        // The pixel data is in 1 bit per pixel format.
        case 1:
        {
            // Loop while there are more pixels to draw.
            while (lCount)
            {
                // Get the next byte of image data.
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data.
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color.
                    LCD_WriteData(((unsigned long *)pucPalette)[(ulByte >> (7 - lX0)) & 1]);
                }

                // Start at the beginning of the next byte of image data.
                lX0 = 0;
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 4 bit per pixel format.
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch (lX0 & 1)
            {
                case 0:
                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        ulByte = (*pucData >> 4) * 3;
                        ulByte = (*(unsigned long *)(pucPalette + ulByte) & 0x00ffffff);

                        // Translate this palette entry and write it to the screen.
                        LCD_WriteData(DPYCOLORTRANSLATE(ulByte));

                        // Decrement the count of pixels to draw.
                        lCount--;

                        // See if there is another pixel to draw.
                        if (lCount)
						{
						    case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            ulByte = (*pucData++ & 15) * 3;
                            ulByte = (*(unsigned long *)(pucPalette + ulByte) &	0x00ffffff);

                            // Translate this palette entry and write it to the
                            // screen.
                            LCD_WriteData(DPYCOLORTRANSLATE(ulByte));

                            // Decrement the count of pixels to draw.
                            lCount--;
                        }
                    }
            }

            // The image data has been drawn.
            break;
        }

        // The pixel data is in 8 bit per pixel format.
        case 8:
        {
            // Loop while there are more pixels to draw.
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                ulByte = *pucData++ * 3;
                ulByte = *(unsigned long *)(pucPalette + ulByte) & 0x00ffffff;

                // Translate this palette entry and write it to the screen.
                LCD_WriteData(DPYCOLORTRANSLATE(ulByte));
            }

            // The image data has been drawn.
            break;
        }
    }
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320LineDrawH                              **
** Description   : Draws a horizontal line.                                        **
**				   This function draws a horizontal line on the display.  The      **
**                 coordinates of the line are assumed to be within the extents of **
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the start of pixel.                 **
**				   lX2 - is the X coordinate of the end of the line.               **
**                 lY - is the Y coordinate of the line.       				       **
**                 ulValue - is the color of the line.                             **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320LineDrawH(void *pvDisplayData, long lX1, long lX2,
                                   long lY, unsigned long ulValue)
{
    // Set the cursor increment to left to right, followed by top to bottom.
    LCD_WriteCommand(0x03);
#ifdef PORTRAIT
    LCD_WriteData(0x0030);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(0x0018);
#endif

    // Set the starting X address of the display cursor.
    LCD_WriteCommand(0x20);
#ifdef PORTRAIT
    LCD_WriteData(lX1);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lX1);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(lY);
#endif

    // Set the Y address of the display cursor.
    LCD_WriteCommand(0x21);
#ifdef PORTRAIT
    LCD_WriteData(lY);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(lX1);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(LCD_HORIZONTAL_MAX - 1 - lX1);
#endif

    // Write the data RAM write command.
    LCD_WriteCommand(0x22);

    // Loop through the pixels of this horizontal line.
    while (lX1++ <= lX2)
    {
        // Write the pixel value.
        LCD_WriteData(ulValue);
    }
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320LineDrawV                              **
** Description   : Draws a vertical line.                                          **
**				   This function draws a vertical line on the display.  The        **
**                 coordinates of the line are assumed to be within the extents of **
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the start of pixel.                 **
**				   lY1 is the Y coordinate of the start of the line.               **
**                 lY2 is the Y coordinate of the end of the line.   		       **
**                 ulValue - is the color of the line.                             **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320LineDrawV(void *pvDisplayData, long lX, long lY1,
                                   long lY2, unsigned long ulValue)
{
    // Set the cursor increment to top to bottom, followed by left to right.
    LCD_WriteCommand(0x03);
#ifdef PORTRAIT
    LCD_WriteData(0x0038);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(0x0020);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(0x0008);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(0x0010);
#endif

    // Set the X address of the display cursor.
    LCD_WriteCommand(0x20);
#ifdef PORTRAIT
    LCD_WriteData(lX);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lY1);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(lY1);
#endif

    // Set the starting Y address of the display cursor.
    LCD_WriteCommand(0x21);
#ifdef PORTRAIT
    LCD_WriteData(lY1);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData(319 - lY1);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(319 - lX);
#endif

    // Write the data RAM write command.
    LCD_WriteCommand(0x22);

    // Loop through the pixels of this vertical line.
    while(lY1++ <= lY2)
    {
        // Write the pixel value.
        LCD_WriteData(ulValue);
    }
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320RectFill                               **
** Description   : Draws a vertical line.                                          **
**				   This function fills a rectangle on the display.  The coordinates**
**                 of the rectangle are assumed to be within the extents of the    **
**                 display, and the rectangle specification is fully inclusive (in **
**                 other words, both sXMin and sXMax are drawn, along with sYMin   **
**                 and sYMax).                                                     **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   pRect - is a pointer to the structure describing the rectangle. **
**                 ulValue - is the color of the rectangle.                        **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320RectFill(void *pvDisplayData, const tRectangle *pRect,
                                  unsigned long ulValue)
{
    long lCount;

    // Write the X extents of the rectangle.
    LCD_WriteCommand(0x50);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sXMax);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(pRect->sYMin);
#endif
    LCD_WriteCommand(0x51);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sXMax);
#endif
#ifdef LANDSCAPE
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(pRect->sYMax);
#endif

    // Write the Y extents of the rectangle.
    LCD_WriteCommand(0x52);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif
    LCD_WriteCommand(0x53);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sYMax);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(pRect->sXMax);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sXMin);
#endif

    // Set the display cursor to the upper left of the rectangle.
    LCD_WriteCommand(0x20);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData(pRect->sYMin);
#endif

    LCD_WriteCommand(0x21);
#ifdef PORTRAIT
    LCD_WriteData(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    LCD_WriteData(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    LCD_WriteData((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif

    // Write the data RAM write command.
    LCD_WriteCommand(0x22);

    // Loop through the pixels of this filled rectangle.
    for (lCount = ((pRect->sXMax - pRect->sXMin + 1) * (pRect->sYMax - pRect->sYMin + 1)); 
	               lCount >= 0; 
				   lCount--)
    {
        // Write the pixel value.
        LCD_WriteData(ulValue);
    }

    // Reset the X extents to the entire screen.
    LCD_WriteCommand(0x50);
    LCD_WriteData(0x0000);
    LCD_WriteCommand(0x51);
    LCD_WriteData(0x00ef);

    // Reset the Y extents to the entire screen.
    LCD_WriteCommand(0x52);
    LCD_WriteData(0x0000);
    LCD_WriteCommand(0x53);
    LCD_WriteData(0x013f);
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320ColorTranslate                         **
** Description   : Translates a 24-bit RGB color to a display driver-specific color**
**				   This function translates a 24-bit RGB color into a value that   **
**                 can be written into the display's frame buffer in order to      **
**                 reproduce that color, or the closest possible approximation of  **
**                 that color.                                                     **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   ulValue - is the 24-bit RGB color.  The least-significant byte  **
**                           is the blue channel, the next byte is the green       **
**                           channel, and the third byte is the red channel.       **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned long S95196x240x320x16_ILI9320ColorTranslate(void *pvDisplayData,
                                        unsigned long ulValue)
{
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    return(DPYCOLORTRANSLATE(ulValue));
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320Flush                                  **
** Description   : Flushes any cached drawing operations.                          **
**				   This functions flushes any cached drawing operations to the     **
**                 display.  This is useful when a local frame buffer is used for  **
**                 drawing operations, and the flush would copy the local frame    **
**                 buffer to the display.  For the ILI932x driver, the flush is a  **
**                 no operation.                                                   **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   ulValue - is the 24-bit RGB color.  The least-significant byte  **
**                           is the blue channel, the next byte is the green       **
**                           channel, and the third byte is the red channel.       **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void S95196x240x320x16_ILI9320Flush(void *pvDisplayData)
{
    // There is nothing to be done.
}

//=====================================================================================
// The graphics library display structure that describes the driver for the
// F02, F03 or F05 variants of the Formike Electronic KWH028Q02 TFT panel with
// ILI932x controllers.
//
//=====================================================================================
const tDisplay g_sS95196x240x320x16_ILI9320 =
{
    sizeof(tDisplay),
    0,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
#else
    LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX,
#endif
    S95196x240x320x16_ILI9320PixelDraw,
    S95196x240x320x16_ILI9320PixelDrawMultiple,
    S95196x240x320x16_ILI9320LineDrawH,
    S95196x240x320x16_ILI9320LineDrawV,
    S95196x240x320x16_ILI9320RectFill,
    S95196x240x320x16_ILI9320ColorTranslate,
    S95196x240x320x16_ILI9320Flush
};

//=======================================================================================
//=======================================================================================

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
