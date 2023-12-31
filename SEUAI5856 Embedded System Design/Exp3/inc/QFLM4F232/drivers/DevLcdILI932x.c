/****************************************************************************************
** File Name  £ºDevLcdIli932x.c                                                        **
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
#include "drivers/DevLcdILI932x.h"

static unsigned short dev_lcd_read_data(void);

/************************************************************************************
** Function Name : dev_lcd_write_reg                                               **
** Description   : Write a particular controller register with a value             **
** Arguments     :                                                                 **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
__forceinline void	dev_lcd_write_reg(unsigned char ucIndex, unsigned short usValue)
{
    HAL_LCD_WRITE_COMMAND(ucIndex);
    HAL_LCD_WRITE_DATA(usValue);
}

/************************************************************************************
** Function Name : dev_lcd_read_reg                                                **
** Description   : Read the value of a register from the ILI932x display controller**
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
__forceinline unsigned short dev_lcd_read_reg(unsigned char ucIndex)
{
    HAL_LCD_WRITE_COMMAND(ucIndex);
	 
    return(HAL_LCD_READ_DATA());
}



/************************************************************************************
** Function Name : dev_lcd_write_data                                              **
** Description   : Writes a data word to the ILI932x.                              **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_write_data(unsigned short usData)
{
	  LCD_SET_RS(HAL_LCD_RS_PIN);

    // Write the data to the data bus.
    HWREG(HAL_LCD_DATAH_PORT + GPIO_O_DATA + (HAL_LCD_DATAH_PINS << 2)) = (usData >> 8);   
    HWREG(HAL_LCD_DATAL_PORT + GPIO_O_DATA + (HAL_LCD_DATAL_PINS << 2)) = (usData);   

    // Assert the write enable signal.
	  LCD_SET_WR(0);

    // Deassert the write enable signal.
    LCD_SET_WR(HAL_LCD_WR_PIN);
}

/************************************************************************************
** Function Name : dev_lcd_read_data                                               **
** Description   : Reads a data word from the ILI932x.                             **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned short dev_lcd_read_data(void)
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
static void dev_lcd_write_command(unsigned char ucData)
{
    // Write the command to the data bus.
	  //LCD_WRITE_DATA_16BIT(ucData);
    HWREG(HAL_LCD_DATAH_PORT + GPIO_O_DATA + (HAL_LCD_DATAH_PINS << 2)) = (ucData >> 8);   
    HWREG(HAL_LCD_DATAL_PORT + GPIO_O_DATA + (HAL_LCD_DATAL_PINS << 2)) = (ucData);   

    // Set the RS signal low, indicating a command.
	  LCD_SET_RS(0);

    // Assert the write enable signal.
	  LCD_SET_WR(0);
 
    // Deassert the write enable signal.
    LCD_SET_WR(HAL_LCD_WR_PIN);

    // Set the RS signal high, indicating that following writes are data.
	  LCD_SET_RS(HAL_LCD_RS_PIN);

}

void lcd_ili932x_flush(void *pvDisplayData)
{
    // There is nothing to be done.
	dev_lcd_write_reg(0x07, 0x0133);

}
/************************************************************************************
** Function Name : dev_lcd_init_ili932x                                            **
** Description   : Initializes the display driver.                                 **
** Arguments     : This function initializes the ILI9320, ILI9325 or ILI9328       **
**                 display controller on the panel, preparing it to display data.  **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_init_ili932x(void)
{
    unsigned long ulClockMS, ulCount;
    unsigned short usController;
	  unsigned short usDat;
		unsigned long ulErro=0;
	
    // Get the current processor clock frequency.
    ulClockMS = ROM_SysCtlClockGet() / (3 * 1000);
	
	  //===========================================================
    HAL_LCD_INIT_GPIO();
	  
	  // Reset LCD
    LCD_SET_RST(HAL_LCD_RST_PIN);					// LCD_RST = 1
	  ROM_SysCtlDelay(5 * ulClockMS);
	  LCD_SET_RST(0);						            // LCD_RST = 0
    ROM_SysCtlDelay(10 * ulClockMS);
	  ROM_SysCtlDelay(10 * ulClockMS);
    LCD_SET_RST(HAL_LCD_RST_PIN);					// LCD_RST = 1


	  //===========================================================
		// LCD PIN Test
		//===========================================================
	
	  LCD_SET_BL(HAL_LCD_BL_PIN);	
		LCD_SET_BL(0);	
		LCD_SET_BL(HAL_LCD_BL_PIN);	
		
		LCD_SET_CS(HAL_LCD_CS_PIN);
  	LCD_SET_CS(0);
		LCD_SET_CS(HAL_LCD_CS_PIN);
	
		LCD_SET_RS(HAL_LCD_RS_PIN);
		LCD_SET_RS(0);
		LCD_SET_RS(HAL_LCD_RS_PIN);
	
	
	  LCD_SET_RD(HAL_LCD_RD_PIN);
		LCD_SET_RD(0);
		LCD_SET_RD(HAL_LCD_RD_PIN);
		
		
		LCD_SET_WR(HAL_LCD_WR_PIN);
		LCD_SET_WR(0);
		LCD_SET_WR(HAL_LCD_WR_PIN);
		
		LCD_SET_DATAH(0xFF);
		LCD_SET_DATAL(0xFF);
		LCD_SET_DATAH(0x00);
		LCD_SET_DATAL(0x00);
		LCD_SET_DATAH(0xFF);
		LCD_SET_DATAL(0xFF);
		
		//HAL_LCD_WRITE_DATA(0x0000);
		//HAL_LCD_WRITE_DATA(0xFFFF);
		//HAL_LCD_WRITE_DATA(0x0000);
		
	
	  //===========================================================
    // Set the LCD control pins to their default values.  This also asserts the
    // LCD reset signal.
	  //===========================================================
		LCD_SET_CS(0);
	  LCD_SET_CS(HAL_LCD_CS_PIN);
		LCD_SET_CS(0);
	
	  LCD_SET_BL(0);	
	  LCD_SET_BL(HAL_LCD_BL_PIN);	
	  LCD_SET_BL(0);	
	
		LCD_SET_CS(0);
	  LCD_SET_DATAH(0);
	  LCD_SET_DATAL(0);
	
	  LCD_SET_RS(HAL_LCD_RS_PIN);
	  LCD_SET_RD(HAL_LCD_RD_PIN);
	  LCD_SET_WR(HAL_LCD_WR_PIN);
	  LCD_SET_BL(HAL_LCD_BL_PIN);	
	
  	// Reset LCD
    LCD_SET_RST(HAL_LCD_RST_PIN);					// LCD_RST = 1
	  ROM_SysCtlDelay(5 * ulClockMS);
	  LCD_SET_RST(0);						            // LCD_RST = 0
    ROM_SysCtlDelay(10 * ulClockMS);
	  ROM_SysCtlDelay(10 * ulClockMS);
    LCD_SET_RST(HAL_LCD_RST_PIN);					// LCD_RST = 1

    // Delay for 50ms while the LCD comes out of reset.
    ROM_SysCtlDelay(50 * ulClockMS);
    // Delay for 10ms while the oscillator stabilizes.
    ROM_SysCtlDelay(10 * ulClockMS);

    // Determine which version of the display controller we are using.
    usController = dev_lcd_read_reg(0x00);

    // Internal timing configuration (common to both ILI9320, ILI9325 andILI9328)
    dev_lcd_write_reg(0xE3, 0x3008);

	  if (usController == 0x9325)
	  {
	      // Set internal timing registers on the ILI9325/8 controller
       	dev_lcd_write_reg(0xE7, 0x0012);
        dev_lcd_write_reg(0xEF, 0x1231);

				// Basic interface configuration (common to all controllers).
				dev_lcd_write_reg(0x01, 0x0100); // set SS and SM bit
				dev_lcd_write_reg(0x02, 0x0700); // set 1 line inversion
				dev_lcd_write_reg(0x03, 0x0030); // set GRAM write direction and BGR=0.
				dev_lcd_write_reg(0x04, 0x0000); // Resize register
				dev_lcd_write_reg(0x08, 0x0202); // set the back porch and front porch
				dev_lcd_write_reg(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
				dev_lcd_write_reg(0x0A, 0x0000); // FMARK function
				dev_lcd_write_reg(0x0C, 0x0001); // RGB interface setting
				dev_lcd_write_reg(0x0D, 0x0000); // Frame marker Position
				dev_lcd_write_reg(0x0F, 0x0000); // RGB interface polarity

				// Power On sequence as provided by display manufacturer.
				dev_lcd_write_reg(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
				dev_lcd_write_reg(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
				dev_lcd_write_reg(0x12, 0x0000); // VREG1OUT voltage
				dev_lcd_write_reg(0x13, 0x0000); // VDV[4:0] for VCOM amplitude

				ROM_SysCtlDelay(200 * ulClockMS);
				// Power on sequence for the ILI9325/8.
				dev_lcd_write_reg(0x10, 0x1690);
				dev_lcd_write_reg(0x11, 0x0227);

				ROM_SysCtlDelay(50 * ulClockMS);

				dev_lcd_write_reg(0x12, 0x001A); // External Reference Voltage = VCI
				ROM_SysCtlDelay(50 * ulClockMS);

				dev_lcd_write_reg(0x13, 0x1400);  // Set VDV[4:0] for VCOM amplitude

				dev_lcd_write_reg(0x29, 0x0024);  // Set VCM[5:0] for VCOMH
				dev_lcd_write_reg(0x2B, 0x000C);

				ROM_SysCtlDelay(50 * ulClockMS);

				// GRAM horizontal and vertical addresses
				dev_lcd_write_reg(0x20, 0x0000);  // GRAM horizontal address
				dev_lcd_write_reg(0x21, 0x0000);  // GRAM Vertical address

				// Adjust the Gamma Curve
				dev_lcd_write_reg(0x30, 0x0007);
				dev_lcd_write_reg(0x31, 0x0604);
				dev_lcd_write_reg(0x32, 0x0007);
				dev_lcd_write_reg(0x35, 0x0504);
				dev_lcd_write_reg(0x36, 0x0808);
				dev_lcd_write_reg(0x37, 0x0007);
				dev_lcd_write_reg(0x38, 0x0301);
				dev_lcd_write_reg(0x39, 0x0007);
				dev_lcd_write_reg(0x3C, 0x0302);
				dev_lcd_write_reg(0x3D, 0x0808);

				ROM_SysCtlDelay(50 * ulClockMS);

				// Set GRAM area
				dev_lcd_write_reg(0x50, 0x0000); // Horizontal GRAM Start Address
				dev_lcd_write_reg(0x51, 0x00EF); // Horizontal GRAM End Address
				dev_lcd_write_reg(0x52, 0x0000); // Vertical GRAM Start Address
				dev_lcd_write_reg(0x53, 0x013F); // Vertical GRAM Start Address

				// Driver output control 2, base image display control and vertical scroll control.
				dev_lcd_write_reg(0x60, 0xA700);
				dev_lcd_write_reg(0x61, 0x0001); // NDL,VLE, REV
				dev_lcd_write_reg(0x6A, 0x0000); // set scrolling line

				// Partial Display Control
				dev_lcd_write_reg(0x80, 0x0000);
				dev_lcd_write_reg(0x81, 0x0000);
				dev_lcd_write_reg(0x82, 0x0000);
				dev_lcd_write_reg(0x83, 0x0000);
				dev_lcd_write_reg(0x84, 0x0000);
				dev_lcd_write_reg(0x85, 0x0000);

				// Panel Control
				dev_lcd_write_reg(0x90, 0x0010);
				dev_lcd_write_reg(0x92, 0x0600);
				dev_lcd_write_reg(0x93, 0x0003);
				dev_lcd_write_reg(0x95, 0x0110);
				dev_lcd_write_reg(0x97, 0x0000);
				dev_lcd_write_reg(0x98, 0x0000);
	  }
    else
    {
        // Enable the internal oscillator (ILI9320 only)
		  	dev_lcd_write_reg(0xE7, 0x0010);
        // Enable the internal oscillator (ILI9320 only)
		  	dev_lcd_write_reg(0x00, 0x0001);
				// Basic interface configuration (common to all controllers).
				dev_lcd_write_reg(0x01, 0x0100); // set SS and SM bit
				usDat = dev_lcd_read_reg(0x00);
			  if(usDat!=0x0100)
					ulErro++;
		
		  	dev_lcd_write_reg(0x02, 0x0700); // set 1 line inversion
				dev_lcd_write_reg(0x03, (1<<12) | (1<<5)|(1<<4)); // set GRAM write direction and BGR=0.
				dev_lcd_write_reg(0x04, 0x0000); // Resize register
				dev_lcd_write_reg(0x08, 0x0207); // set the back porch and front porch
				dev_lcd_write_reg(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
				dev_lcd_write_reg(0x0A, 0x0000); // FMARK function
				dev_lcd_write_reg(0x0C, 0x0001); // RGB interface setting
				dev_lcd_write_reg(0x0D, 0x0000); // Frame marker Position
				dev_lcd_write_reg(0x0F, 0x0000); // RGB interface polarity

				// Power On sequence as provided by display manufacturer.
				dev_lcd_write_reg(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
				dev_lcd_write_reg(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
				dev_lcd_write_reg(0x12, 0x0000); // VREG1OUT voltage
				dev_lcd_write_reg(0x13, 0x0000); // VDV[4:0] for VCOM amplitude

				ROM_SysCtlDelay(200 * ulClockMS);
				
				// Power on sequence for the ILI9320.
					dev_lcd_write_reg(0x10, 0x1590);
					dev_lcd_write_reg(0x11, 0x0227);

					ROM_SysCtlDelay(50 * ulClockMS);

					dev_lcd_write_reg(0x12, 0x009C);

					ROM_SysCtlDelay(50 * ulClockMS);

					dev_lcd_write_reg(0x13, 0x1900);
					dev_lcd_write_reg(0x29, 0x0023);
					dev_lcd_write_reg(0x2B, 0x000e);
					
					ROM_SysCtlDelay(50 * ulClockMS);
				
				// GRAM horizontal and vertical addresses
				dev_lcd_write_reg(0x20, 0x0000);  // GRAM horizontal address
				dev_lcd_write_reg(0x21, 0x0000);  // GRAM Vertical address

				// Adjust the Gamma Curve
				dev_lcd_write_reg(0x30, 0x0007);
				dev_lcd_write_reg(0x31, 0x0707);
				dev_lcd_write_reg(0x32, 0x0006);
				dev_lcd_write_reg(0x35, 0x0704);
				dev_lcd_write_reg(0x36, 0x1F04);
				dev_lcd_write_reg(0x37, 0x0004);
				dev_lcd_write_reg(0x38, 0x0000);
				dev_lcd_write_reg(0x39, 0x0706);
				dev_lcd_write_reg(0x3C, 0x0701);
				dev_lcd_write_reg(0x3D, 0x000F);

				ROM_SysCtlDelay(50 * ulClockMS);

				// Set GRAM area
				dev_lcd_write_reg(0x50, 0x0000); // Horizontal GRAM Start Address
				dev_lcd_write_reg(0x51, 0x00EF); // Horizontal GRAM End Address
				dev_lcd_write_reg(0x52, 0x0000); // Vertical GRAM Start Address
				dev_lcd_write_reg(0x53, 0x013F); // Vertical GRAM Start Address

				// Driver output control 2, base image display control and vertical scroll control.
				dev_lcd_write_reg(0x60, 0xA700);
				dev_lcd_write_reg(0x61, 0x0001); // NDL,VLE, REV
				dev_lcd_write_reg(0x6A, 0x0000); // set scrolling line

				// Partial Display Control
				dev_lcd_write_reg(0x80, 0x0000);
				dev_lcd_write_reg(0x81, 0x0000);
				dev_lcd_write_reg(0x82, 0x0000);
				dev_lcd_write_reg(0x83, 0x0000);
				dev_lcd_write_reg(0x84, 0x0000);
				dev_lcd_write_reg(0x85, 0x0000);

				// Panel Control
				dev_lcd_write_reg(0x90, 0x0010);
				dev_lcd_write_reg(0x92, 0x0000);
				dev_lcd_write_reg(0x93, 0x0003);
				dev_lcd_write_reg(0x95, 0x0110);
				dev_lcd_write_reg(0x97, 0x0000);
				dev_lcd_write_reg(0x98, 0x0000);
    }

    // Clear the contents of the display buffer.
    HAL_LCD_WRITE_COMMAND(0x22);
    for (ulCount = 0; ulCount < (320 * 240); ulCount++)
    {
        HAL_LCD_WRITE_DATA(0x0FFF);
    }
	  // test..................2012/03/08
    // Enable the image display.
    dev_lcd_write_reg(0x07, 0x0133);

    // Delay for 20ms, which is equivalent to two frames.
    ROM_SysCtlDelay(20 * ulClockMS);

    // DEV_LCD_ILI832X_REFRESH();
}


/************************************************************************************
** Function Name : dev_lcd_ili932x_rect_fill                                       **
** Description   : Draws a vertical line.                                          **
**				   This function fills a rectangle on the display.  The coordinates      **
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
** Data          : 2013-03-10                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_ili932x_rect_fill(unsigned long x1,unsigned long y1,unsigned long x2,unsigned long y2, unsigned long ulValue)
{
    long lCount;

    // Write the X extents of the rectangle.
    dev_lcd_write_command(0x50);
#ifdef PORTRAIT
    dev_lcd_write_data(x1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - y2);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - x2);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(y1);
#endif

    dev_lcd_write_command(0x51);
#ifdef PORTRAIT
    dev_lcd_write_data(x2);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - y1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - x1);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(y2);
#endif

    // Write the Y extents of the rectangle.
    dev_lcd_write_command(0x52);
#ifdef PORTRAIT
    dev_lcd_write_data(y1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(x1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - y2);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - x2);
#endif

    dev_lcd_write_command(0x53);
#ifdef PORTRAIT
    dev_lcd_write_data(y2);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(x2);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - y1);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - x1);
#endif

    // Set the display cursor to the upper left of the rectangle.
    dev_lcd_write_command(0x20);
#ifdef PORTRAIT
    dev_lcd_write_data(x1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - y1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_VERTICAL_MAX - 1) - x1);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(y1);
#endif

    dev_lcd_write_command(0x21);
#ifdef PORTRAIT
    dev_lcd_write_data(y1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(x1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - y2);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data((LCD_HORIZONTAL_MAX - 1) - x2);
#endif

    // Write the data RAM write command.
    dev_lcd_write_command(0x22);

    // Loop through the pixels of this filled rectangle.
    for (lCount = ((x2 - x1 + 1) * (y2 - y1 + 1)); 
	               lCount >= 0; 
				   lCount--)
    {
        // Write the pixel value.
        dev_lcd_write_data(ulValue);
    }

    // Reset the X extents to the entire screen.
    dev_lcd_write_command(0x50);
    dev_lcd_write_data(0x0000);
    dev_lcd_write_command(0x51);
    dev_lcd_write_data(0x00ef);

    // Reset the Y extents to the entire screen.
    dev_lcd_write_command(0x52);
    dev_lcd_write_data(0x0000);
    dev_lcd_write_command(0x53);
    dev_lcd_write_data(0x013f);
}
/************************************************************************************
** Function Name : dev_lcd_ili2x_clear                                             **
** Description   : Read the value of a register from the ILI932x display controller**
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_ili932x_clear(unsigned long ulcolor)
{
    dev_lcd_ili932x_rect_fill(0,0,LCD_VERTICAL_MAX-1,LCD_HORIZONTAL_MAX-1,ulcolor);	
}

/************************************************************************************
** Function Name : dev_lcd_ili932x_line_draw_ver                                   **
** Description   : Draws a vertical line.                                          **
**				   This function draws a vertical line on the display.  The              **
**                 coordinates of the line are assumed to be within the extents of **
**                 the display.                                                    **
** Arguments     : pvDisplayData -is a pointer to the driver-specific data for this**
**                                display driver.                                  **
**				   lX - is the X coordinate of the start of pixel.                       **
**				   lY1 is the Y coordinate of the start of the line.                     **
**                 lY2 is the Y coordinate of the end of the line.   		           **
**                 ulValue - is the color of the line.                             **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_ili932x_line_draw_ver(long lX, long lY1,
                                   long lY2, unsigned long ulValue)
{
    // Set the cursor increment to top to bottom, followed by left to right.
    dev_lcd_write_command(0x03);
#ifdef PORTRAIT
    dev_lcd_write_data(0x0038);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(0x0020);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(0x0008);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(0x0010);
#endif

    // Set the X address of the display cursor.
    dev_lcd_write_command(0x20);
#ifdef PORTRAIT
    dev_lcd_write_data(lX);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(LCD_VERTICAL_MAX - 1 - lY1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(LCD_VERTICAL_MAX - 1 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(lY1);
#endif

    // Set the starting Y address of the display cursor.
    dev_lcd_write_command(0x21);
#ifdef PORTRAIT
    dev_lcd_write_data(lY1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(lX);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(319 - lY1);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(319 - lX);
#endif

    // Write the data RAM write command.
    dev_lcd_write_command(0x22);

    // Loop through the pixels of this vertical line.
    while (lY1++ <= lY2)
    {
        // Write the pixel value.
        dev_lcd_write_data(ulValue);
    }
}

/************************************************************************************
** Function Name : dev_lcd_ili932x_line_draw_hor                                       **
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
void dev_lcd_ili932x_line_draw_hor(long lX1, long lX2,
                                   long lY, unsigned long ulValue)
{
    // Set the cursor increment to left to right, followed by top to bottom.
    dev_lcd_write_command(0x03);
#ifdef PORTRAIT
    dev_lcd_write_data(0x0030);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(0x0018);
#endif

    // Set the starting X address of the display cursor.
    dev_lcd_write_command(0x20);
#ifdef PORTRAIT
    dev_lcd_write_data(lX1);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(LCD_VERTICAL_MAX - 1 - lY);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(LCD_VERTICAL_MAX - 1 - lX1);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(lY);
#endif

    // Set the Y address of the display cursor.
    dev_lcd_write_command(0x21);
#ifdef PORTRAIT
    dev_lcd_write_data(lY);
#endif
#ifdef LANDSCAPE
    dev_lcd_write_data(lX1);
#endif
#ifdef PORTRAIT_FLIP
    dev_lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    dev_lcd_write_data(LCD_HORIZONTAL_MAX - 1 - lX1);
#endif

    // Write the data RAM write command.
    dev_lcd_write_command(0x22);

    // Loop through the pixels of this horizontal line.
    while (lX1++ <= lX2)
    {
        // Write the pixel value.
        dev_lcd_write_data(ulValue);
    }
}



/************************************************************************************
** Function Name : dev_lcd_ili932x_DrawRectangle                                   **
** Description   : Read the value of a register from the ILI932x display controller**
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2013-01-05                                                      **
** Notes         :                                                                 **
************************************************************************************/
void dev_lcd_ili932x_draw_rectangle(unsigned long x1,unsigned long y1,
                                   unsigned long x2,unsigned long y2,
                                   unsigned long ulValue,unsigned char mode)
{

	if (mode)
	{

		dev_lcd_ili932x_rect_fill(x1,y1,x2,y2,ulValue);
	}
	else
	{
		dev_lcd_ili932x_line_draw_ver(x1,y1,y2,ulValue);
		dev_lcd_ili932x_line_draw_ver(x2,y1,y2,ulValue);
		dev_lcd_ili932x_line_draw_hor(x1,x2,y1,ulValue);
		dev_lcd_ili932x_line_draw_hor(x1,x2,y2,ulValue);
	}
}

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
