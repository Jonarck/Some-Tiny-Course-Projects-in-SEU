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

#include "drivers/lcd_320x240_ili932x.h"

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
static void lcd_write_data(unsigned short usData)
{
	LCD_SET_RS(LCD_RS_PIN);

    // Write the data to the data bus.
    //LCD_WRITE_DATA_16BIT(usData);
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (usData >> 8);   
    HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (usData);   

    // Assert the write enable signal.
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
static unsigned short lcd_read_data(void)
{
    unsigned short usData;

    // Make the data bus be an input.
	ROM_GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_IN);
	ROM_GPIODirModeSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_DIR_MODE_IN);



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
static void lcd_write_command(unsigned char ucData)
{
    // Write the command to the data bus.
	//LCD_WRITE_DATA_16BIT(ucData);
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucData >> 8);   
    HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (ucData);   

    // Set the RS signal low, indicating a command.
	LCD_SET_RS(0);

    // Assert the write enable signal.
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
__forceinline unsigned short LCD_ReadRegister(unsigned char ucIndex)
{
    lcd_write_command(ucIndex);
    return(lcd_read_data());
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
__forceinline void	LCD_WriteRegister(unsigned char ucIndex, unsigned short usValue)
{
    lcd_write_command(ucIndex);
    lcd_write_data(usValue);
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
static void lcd_init_gpio(void)
{
	// Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Configure the pins that connect to the LCD as GPIO outputs.
    ROM_GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, LCD_DATAH_PINS);
	  ROM_GPIOPinTypeGPIOOutput(LCD_DATAL_BASE, LCD_DATAL_PINS);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTJ_BASE, LCD_LEDK_PIN );
    
   
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, LCD_RS_PIN | LCD_RD_PIN);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, LCD_CS_PIN | LCD_WR_PIN);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_6);
 
	  ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0x00);   		    	// LCD_LED_K1 = 0

	// PD.7 default as NMI Port
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
    
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
 	
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	
}


//*****************************************************************************
//
//! Determines whether an ILI9320, ILI9325 or ILI9328 controller is present.
//!
//! This function queries the ID of the display controller in use and returns
//! it to the caller.  This driver supports both ILI9320, ILI9325 and ILI9328.
//! These are very similar but the sense of the long display axis is reversed
//! in the Formike KWH028Q02-F03 using an ILI9320 relative to the other two
//! supported displays and this information is needed by the touchscreen driver
//! to provide correct touch coordinate information.
//!
//! \return Returns 0x9320 if an ILI9320 controller is in use, 0x9325 if an
//! ILI9325 is present or 0x9328 if an ILI9328 is detected.
//
//*****************************************************************************
unsigned short lcd_get_control(void)
{
    unsigned short usController;

    // Determine which version of the display controller we are using.
    usController = LCD_ReadRegister(0x00);

    return(usController);
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
void lcd_ili932x_init(void)
{
   unsigned long ulClockMS,ulCount;
    unsigned short usController;

    // Get the current processor clock frequency.
    ulClockMS = ROM_SysCtlClockGet() / (3 * 1000);
	
	//===========================================================
    lcd_init_gpio();

	//===========================================================
    // Set the LCD control pins to their default values.  This also asserts the
    // LCD reset signal.
	//==============================================================================================
	//==============================================================================================
		
	  ROM_GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0x00);						    // LCD_CS = 0
    ROM_GPIOPinWrite(LCD_DATAH_BASE, LCD_DATAH_PINS, 0x00);						// LCD_DATA_H = 0
    ROM_GPIOPinWrite(LCD_DATAL_BASE, LCD_DATAL_PINS, 0x00);						// LCD_DATA_L = 0

    ROM_GPIOPinWrite(LCD_RS_BASE, LCD_RS_PIN, LCD_RS_PIN);						// LCD_RS = 0
    ROM_GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);						// LCD_RD = 0
    ROM_GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);						// LCD_WR = 0
	  ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0);   		    	// LCD_LED_K1 = 1

	// Reset LCD
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);					// LCD_RST = 0
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


	    // Set internal timing registers on the ILI9325/8 controller
       	LCD_WriteRegister(0x00e7,0x0010);
        LCD_WriteRegister(0x0000,0x0001);

 

    // Basic interface configuration (common to all controllers).
    LCD_WriteRegister(0x01, 0x0100); // set SS and SM bit
    LCD_WriteRegister(0x02, 0x0700); // set 1 line inversion
    LCD_WriteRegister(0x03, (1<<12)|(1<<5)|(1<<4) ); // set GRAM write direction and BGR=0.
    LCD_WriteRegister(0x04, 0x0000); // Resize register
    LCD_WriteRegister(0x08, 0x0207); // set the back porch and front porch
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
    
        // Power on sequence for the ILI9325/8.
        LCD_WriteRegister(0x10, 0x1590);
        LCD_WriteRegister(0x11, 0x0227);

        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x12, 0x009c); // External Reference Voltage = VCI
        ROM_SysCtlDelay(50 * ulClockMS);

        LCD_WriteRegister(0x13, 0x1900);  // Set VDV[4:0] for VCOM amplitude
        LCD_WriteRegister(0x29, 0x0023);  // Set VCM[5:0] for VCOMH
        LCD_WriteRegister(0x2B, 0x000e);

        ROM_SysCtlDelay(50 * ulClockMS);
    
    

    // GRAM horizontal and vertical addresses
    LCD_WriteRegister(0x20, 0x0000);  // GRAM horizontal address
    LCD_WriteRegister(0x21, 0x0000);  // GRAM Vertical address

    // Adjust the Gamma Curve
    LCD_WriteRegister(0x30,  0x0007 );
    LCD_WriteRegister(0x31,  0x0707);
    LCD_WriteRegister(0x32, 0x0006);
    LCD_WriteRegister(0x35, 0x0704);
    LCD_WriteRegister(0x36, 0x1f04);
    LCD_WriteRegister(0x37, 0x0004);
    LCD_WriteRegister(0x38, 0x0000);
    LCD_WriteRegister(0x39, 0x0706);
    LCD_WriteRegister(0x3C, 0x0701);
    LCD_WriteRegister(0x3D,0x000f);
    ROM_SysCtlDelay(50 * ulClockMS);
    // Set GRAM area
    LCD_WriteRegister(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_WriteRegister(0x51, 0x00EF); // Horizontal GRAM End Address
    LCD_WriteRegister(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_WriteRegister(0x53, 0x013F); // Vertical GRAM Start Address

    // Driver output control 2, base image display control and vertical scroll control.
    LCD_WriteRegister(0x60,  0xA700 );
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

// 	  LCD_WriteRegister(0x0007,0x0133);
// 	
// 	  LCD_WriteRegister(0x0020,0x0000);                                                            
// 	  LCD_WriteRegister(0x0021,0x0000);
		
		
// 	LCD_WriteRegister(0x0001, 0x0100); 
// 	//wr_reg(0x0001, 0x0000); 
// 	LCD_WriteRegister(0x0002, 0x0700); 
// 	LCD_WriteRegister(0x0003, 0x0030); 
// 	//wr_reg(0x0003, 0x0030); BGR
// 	LCD_WriteRegister(0x0004, 0x0000); 
// 	LCD_WriteRegister(0x0008, 0x0202); 
// 	LCD_WriteRegister(0x0009, 0x0000); 
// 	LCD_WriteRegister(0x000A, 0x0000); 
// 	LCD_WriteRegister(0x000C, 0x0000); 
// 	LCD_WriteRegister(0x000D, 0x0000); 
// 	LCD_WriteRegister(0x000F, 0x0000); 
// //*************Power On sequence ****************//
// 	LCD_WriteRegister(0x0010, 0x0000); 
// 	LCD_WriteRegister(0x0011, 0x0007);
// 	LCD_WriteRegister(0x0012, 0x0000); 
// 	LCD_WriteRegister(0x0013, 0x0000); 
// 	LCD_WriteRegister(0x0007, 0x0001);
// 	for(i=60000;i>0;i--);
// 	LCD_WriteRegister(0x0010, 0x1690); 
// 	LCD_WriteRegister(0x0011, 0x0227);
// 	for(i=50000;i>0;i--);
// 	LCD_WriteRegister(0x0012, 0x001A); 
// 	for(i=50000;i>0;i--);
// 	LCD_WriteRegister(0x0013, 0x1400); 
// 	LCD_WriteRegister(0x0029, 0x0024); 
// 	LCD_WriteRegister(0x002B, 0x000C); 
// 	for(i=50000;i>0;i--);
// 	LCD_WriteRegister(0x0020, 0x0000); 
// 	LCD_WriteRegister(0x0021, 0x0000); 
// // ----------- Adjust the Gamma Curve ----------//
// 	LCD_WriteRegister(0x0030, 0x0000);
// 	LCD_WriteRegister(0x0031, 0x0707);
// 	LCD_WriteRegister(0x0032, 0x0307);
// 	LCD_WriteRegister(0x0035, 0x0200);
// 	LCD_WriteRegister(0x0036, 0x0008);
// 	LCD_WriteRegister(0x0037, 0x0004);
// 	LCD_WriteRegister(0x0038, 0x0000);
// 	LCD_WriteRegister(0x0039, 0x0707);
// 	LCD_WriteRegister(0x003C, 0x0002);
// 	LCD_WriteRegister(0x003D, 0x1D04);
// //------------------ Set GRAM area ---------------//
// 	LCD_WriteRegister(0x0050, 0x0000);
// 	LCD_WriteRegister(0x0051, 0x00EF); 
// 	LCD_WriteRegister(0x0052, 0x0000); 
// 	LCD_WriteRegister(0x0053, 0x013F); 
// 	LCD_WriteRegister(0x0060, 0xA700); 
// 	LCD_WriteRegister(0x0061, 0x0000);//general TFT
// 	//wr_reg(0x0061, 0x0000);//IPS TFT
// 	LCD_WriteRegister(0x006A, 0x0000); 
// //-------------- Partial Display Control ---------//
// 	LCD_WriteRegister(0x0080, 0x0000);
// 	LCD_WriteRegister(0x0081, 0x0000);
// 	LCD_WriteRegister(0x0082, 0x0000);
// 	LCD_WriteRegister(0x0083, 0x0000);
// 	LCD_WriteRegister(0x0084, 0x0000);
// 	LCD_WriteRegister(0x0085, 0x0000);
// //-------------- Panel Control -------------------//
// 	LCD_WriteRegister(0x0090, 0x0010);
// 	LCD_WriteRegister(0x0092, 0x0600);
// 	LCD_WriteRegister(0x0007, 0x0133); // ¿ªÆôÏÔÊ¾
	
 // Clear the contents of the display buffer.
    lcd_write_command(0x22);
    for (ulCount = 0; ulCount < (320 * 240); ulCount++)
    {
        lcd_write_data(0x0000);
    }
	// test..................2012/03/08
    // Enable the image display.
    //LCD_WriteRegister(0x07, 0x0133);

    // Delay for 20ms, which is equivalent to two frames.
    ROM_SysCtlDelay(20 * ulClockMS);
}

/************************************************************************************
** Function Name : lcd_backlight_on                                                **
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
void lcd_backlight_on(void)
{
    // Deassert the signal that turns on the backlight.
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, LCD_LEDK_PIN);
}

/************************************************************************************
** Function Name : lcd_backlight_off                                               **
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
void lcd_backlight_off(void)
{
    // Deassert the signal that turns off the backlight.
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0);
}

/************************************************************************************
** Function Name : lcd_ili932x_pixel_draw                                          **
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
static void lcd_ili932x_pixel_draw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
    // Set the X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the pixel value.
    lcd_write_command(0x22);
    lcd_write_data(ulValue);
}


/************************************************************************************
** Function Name : lcd_ili932x_pixel_draw_multiple                                 **
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
static void lcd_ili932x_pixel_draw_multiple(void *pvDisplayData, long lX,
                                           long lY, long lX0, long lCount,
                                           long lBPP,
                                           const unsigned char *pucData,
                                           const unsigned char *pucPalette)
{
    unsigned long ulByte;

    // Set the cursor increment to left to right, followed by top to bottom.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0030);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0018);
#endif

    // Set the starting X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

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
                    lcd_write_data(((unsigned long *)pucPalette)[(ulByte >> (7 - lX0)) & 1]);
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
                        lcd_write_data(DPYCOLORTRANSLATE(ulByte));

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
                            lcd_write_data(DPYCOLORTRANSLATE(ulByte));

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
                lcd_write_data(DPYCOLORTRANSLATE(ulByte));
            }

            // The image data has been drawn.
            break;
        }
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_line_draw_hor                                       **
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
static void lcd_ili932x_line_draw_hor(void *pvDisplayData, long lX1, long lX2,
                                   long lY, unsigned long ulValue)
{
    // Set the cursor increment to left to right, followed by top to bottom.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0030);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0018);
#endif

    // Set the starting X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX1);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX1);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX1);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX1);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this horizontal line.
    while (lX1++ <= lX2)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_line_draw_ver                                       **
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
static void lcd_ili932x_line_draw_ver(void *pvDisplayData, long lX, long lY1,
                                   long lY2, unsigned long ulValue)
{
    // Set the cursor increment to top to bottom, followed by left to right.
    lcd_write_command(0x03);
#ifdef PORTRAIT
    lcd_write_data(0x0038);
#endif
#ifdef LANDSCAPE
    lcd_write_data(0x0020);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(0x0008);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(0x0010);
#endif

    // Set the X address of the display cursor.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lY1);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(lY1);
#endif

    // Set the starting Y address of the display cursor.
    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(lY1);
#endif
#ifdef LANDSCAPE
    lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data(319 - lY1);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(319 - lX);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this vertical line.
    while(lY1++ <= lY2)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }
}

/************************************************************************************
** Function Name : lcd_ili932x_rect_fill                                           **
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
static void lcd_ili932x_rect_fill(void *pvDisplayData, const tRectangle *pRect, unsigned long ulValue)
{
    long lCount;

    // Write the X extents of the rectangle.
    lcd_write_command(0x50);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMin);
#endif
    lcd_write_command(0x51);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMax);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMax);
#endif

    // Write the Y extents of the rectangle.
    lcd_write_command(0x52);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif
    lcd_write_command(0x53);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMax);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMax);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMin);
#endif

    // Set the display cursor to the upper left of the rectangle.
    lcd_write_command(0x20);
#ifdef PORTRAIT
    lcd_write_data(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_VERTICAL_MAX - 1) - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data(pRect->sYMin);
#endif

    lcd_write_command(0x21);
#ifdef PORTRAIT
    lcd_write_data(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    lcd_write_data(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    lcd_write_data((LCD_HORIZONTAL_MAX - 1) - pRect->sXMax);
#endif

    // Write the data RAM write command.
    lcd_write_command(0x22);

    // Loop through the pixels of this filled rectangle.
    for (lCount = ((pRect->sXMax - pRect->sXMin + 1) * (pRect->sYMax - pRect->sYMin + 1)); 
	               lCount >= 0; 
				   lCount--)
    {
        // Write the pixel value.
        lcd_write_data(ulValue);
    }

    // Reset the X extents to the entire screen.
    lcd_write_command(0x50);
    lcd_write_data(0x0000);
    lcd_write_command(0x51);
    lcd_write_data(0x00ef);

    // Reset the Y extents to the entire screen.
    lcd_write_command(0x52);
    lcd_write_data(0x0000);
    lcd_write_command(0x53);
    lcd_write_data(0x013f);
}

/************************************************************************************
** Function Name : lcd_ili932x_color_translate                                     **
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
static unsigned long lcd_ili932x_color_translate(void *pvDisplayData, unsigned long ulValue)
{
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    return(DPYCOLORTRANSLATE(ulValue));
	//return 0;
}

/************************************************************************************
** Function Name : lcd_ili932x_flush                                               **
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
void lcd_ili932x_flush(void *pvDisplayData)
{
    // There is nothing to be done.
	LCD_WriteRegister(0x07, 0x0133);

}

/************************************************************************************
** Function Name : lcd_ili932x_close                                               **
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
void lcd_ili932x_close(void *pvDisplayData)
{
	LCD_WriteRegister(0x07, 0x0000);
}
//=====================================================================================
// The graphics library display structure that describes the driver for the
// F02, F03 or F05 variants of the Formike Electronic KWH028Q02 TFT panel with
// ILI932x controllers.
//
//=====================================================================================
const tDisplay g_slcd_320x240_ili932x =
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
    lcd_ili932x_pixel_draw,
    lcd_ili932x_pixel_draw_multiple,
    lcd_ili932x_line_draw_hor,
    lcd_ili932x_line_draw_ver,
    lcd_ili932x_rect_fill,
    lcd_ili932x_color_translate,
    lcd_ili932x_flush
};


//=======================================================================================
//=======================================================================================
