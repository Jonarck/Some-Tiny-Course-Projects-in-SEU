//*****************************************************************************
//
// lcd240x320x16_8bit.c - Display driver for LCD controller.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 5961 of the RDK-IDM Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "drivers/lcd240x320x16_8bit.h"

//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide, and the flex connector is
//              on the bottom of the display.  This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall, and the flex connector is
//               on the right side of the display.  This is selected by
//               defining LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide, and the flex
//                   connector is on the top of the display.  This is selected
//                   by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall, and the flex
//                    connector is on the left side of the display.  This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if portrait mode is
// 0 degrees of screen rotation, landscape is 90 degrees of counter-clockwise
// rotation, portrait flip is 180 degrees of rotation, and landscape flip is
// 270 degress of counter-clockwise rotation.
//
// If no screen orientation is selected, portrait mode will be used.
//
//*****************************************************************************
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define LANDSCAPE//PORTRAIT
#endif

unsigned short g_usController;

//*****************************************************************************
//
// Defines for the pins that are used to communicate with the LGDP4535.
//
//*****************************************************************************
#define LCD_DATA_PINS          0xFF

#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOD
#define LCD_RST_PERIPH          SYSCTL_PERIPH_GPIOB
#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOJ
#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOH
#define LCD_RS_PERIPH           SYSCTL_PERIPH_GPIOF
#define LCD_BL_PERIPH           SYSCTL_PERIPH_GPIOH

#define LCD_RST_BASE            GPIO_PORTB_BASE
#define LCD_RST_PIN             GPIO_PIN_7
#define LCD_DATAH_BASE          GPIO_PORTD_BASE
#define LCD_RS_BASE             GPIO_PORTH_BASE
#define LCD_RS_PIN              GPIO_PIN_7
#define LCD_RD_BASE             GPIO_PORTJ_BASE
#define LCD_RD_PIN              GPIO_PIN_7
#define LCD_WR_BASE             GPIO_PORTH_BASE
#define LCD_WR_PIN              GPIO_PIN_6
#define LCD_BL_BASE             GPIO_PORTH_BASE
#define LCD_BL_PIN              GPIO_PIN_1

//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color.
//
// \return Returns the display-driver specific color.
//
//*****************************************************************************
#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00ff0000) >> 19) |               \
                                 ((((c) & 0x0000ff00) >> 5) & 0x000007e0) | \
                                 ((((c) & 0x000000ff) << 8) & 0x0000f800))

//*****************************************************************************
//
// Writes a data word to the ILI932x.
//
//*****************************************************************************
static void
WriteData(unsigned short usData)
{
    //
    // Write the data to the data bus.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) = usData >> 8;

    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;

    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;

    //
    // Write the data to the data bus.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) = usData;

    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;

    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
}

//*****************************************************************************
//
// Reads a data word from the ILI932x.
//
//*****************************************************************************
static unsigned short
ReadData(void)
{
    unsigned short usData;

    //
    // Make the data bus be an input.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DIR) = 0x00;

    //
    // Assert the read signal.  This is done multiple times (though only the
    // first has an affect on the pin) in order to meet the timing requirements
    // of the ILI932x.
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;

    //
    // Read the data from the data bus.
    //
    usData = HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) << 8;

    //
    // Deassert the read signal.
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;


    //
    // Assert the read signal.  This is done multiple times (though only the
    // first has an affect on the pin) in order to meet the timing requirements
    // of the ILI932x.
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;

    //
    // Read the data from the data bus.
    //
    usData |= HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2));

    //
    // Deassert the read signal.
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;


    //
    // Change the data bus back to an output.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DIR) = 0xff;
    //
    // Return the data that was read.
    //
    return(usData);
}

//*****************************************************************************
//
// Writes a command to the ILI932x.
//
//*****************************************************************************
static void
WriteCommand(unsigned char ucData)
{
    //
    // Write the command to the data bus.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) = 0x00;

    //
    // Set the RS signal low, indicating a command.
    //
    HWREG(LCD_RS_BASE + GPIO_O_DATA + (LCD_RS_PIN << 2)) = 0;

    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;

    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;

    //
    // Write the command to the data bus.
    //
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) = ucData;

    //
    // Set the RS signal low, indicating a command.
    //
    HWREG(LCD_RS_BASE + GPIO_O_DATA + (LCD_RS_PIN << 2)) = 0;

    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;

    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;

    //
    // Set the RS signal high, indicating that following writes are data.
    //
    HWREG(LCD_RS_BASE + GPIO_O_DATA + (LCD_RS_PIN << 2)) = LCD_RS_PIN;
}

//*****************************************************************************
//
// Read the value of a register from the ILI932x display controller.
//
//*****************************************************************************
static unsigned short
ReadRegister(unsigned char ucIndex)
{
    WriteCommand(ucIndex);
	ReadData();
    return(ReadData());
}

//*****************************************************************************
//
// Read the value of a register from the ILI932x display controller.
//
//*****************************************************************************
static unsigned short
ReadILI9338Register(unsigned char ucIndex)
{
    WriteCommand(ucIndex);
	ReadData();
    return(ReadData());
}

//*****************************************************************************
//
// Write a particular controller register with a value.
//
//*****************************************************************************
static void
WriteRegister(unsigned char ucIndex, unsigned short usValue)
{
    WriteCommand(ucIndex);
    WriteData(usValue);
}

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the LGDP4535 display
//! controller on the panel, preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void
Lcd240x320x16_8bitInit(void)
{
    unsigned long ulClockMS, ulCount;
    //
    // Get the current processor clock frequency.
    //
    ulClockMS = SysCtlClockGet() / (3 * 1000);

    //
    // Enable the GPIO peripherals used to interface to the ILI932x.
    //

    SysCtlPeripheralEnable(LCD_DATAH_PERIPH);
    SysCtlPeripheralEnable(LCD_RST_PERIPH);
    SysCtlPeripheralEnable(LCD_RD_PERIPH);
    SysCtlPeripheralEnable(LCD_WR_PERIPH);
	SysCtlPeripheralEnable(LCD_RS_PERIPH);
	SysCtlPeripheralEnable(LCD_BL_PERIPH);

    //
    // Convert the PB7/NMI pin into a GPIO pin.  This requires the use of the
    // GPIO lock since changing the state of the pin is otherwise disabled.
    //
    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    HWREG(GPIO_PORTB_BASE + GPIO_O_CR) = 0x80;

    //
    // Make PB7 an output.
    //
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);

    //
    // Clear the commit register, effectively locking access to registers
    // controlling the PB7 configuration.
    //
    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    HWREG(GPIO_PORTB_BASE + GPIO_O_CR) = 0x00;

    //
    // Configure the pins that connect to the LCD as GPIO outputs.
    //
    GPIODirModeSet(LCD_DATAH_BASE, LCD_DATA_PINS, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_DATAH_BASE, LCD_DATA_PINS, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);
    GPIODirModeSet(LCD_RS_BASE, LCD_RS_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RS_BASE, LCD_RS_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);
    GPIODirModeSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);
    GPIODirModeSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);
    GPIODirModeSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);
    GPIODirModeSet(LCD_BL_BASE, LCD_BL_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_BL_BASE, LCD_BL_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD);


    //
    // Set the LCD control pins to their default values.  This also asserts the
    // LCD reset signal.
    //
    GPIOPinWrite(LCD_DATAH_BASE, LCD_DATA_PINS, 0x00);
    GPIOPinWrite(LCD_RS_BASE, LCD_RS_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0x00);
    GPIOPinWrite(LCD_BL_BASE, LCD_BL_PIN, 0x00);


    //
    // Delay for 10ms.
    //
    SysCtlDelay(10 * ulClockMS);

    //
    // Deassert the LCD reset signal.
    //
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);

    //
    // Delay for 50ms while the LCD comes out of reset.
    //
    SysCtlDelay(50 * ulClockMS);

    //
    // Delay for 10ms while the oscillator stabilizes.
    //
    SysCtlDelay(10 * ulClockMS); 

    g_usController = ReadRegister(0x00);

	if (g_usController == 0x9331)
	{
//	    WriteRegister(0xE7, 0x1014);
//-----Power On sequence-----------------------	
	    //
	    // Basic interface configuration (common to all controllers).
	    //
	    WriteRegister(0x01, 0x0100); // set SS and SM bit
	    WriteRegister(0x02, 0x0200); // set 1 line inversion
	    WriteRegister(0x03, 0x0038); // set GRAM write direction and BGR=0.
	    WriteRegister(0x08, 0x0202); // set the back porch and front porch
	    WriteRegister(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
	    WriteRegister(0x0A, 0x0000); // FMARK function
	    WriteRegister(0x0C, 0x0000); // RGB interface setting
	    WriteRegister(0x0D, 0x0000); // Frame marker Position
	    WriteRegister(0x0F, 0x0000); // RGB interface polarity
	
	    //
	    // Power On sequence as provided by display manufacturer.
	    //
	    WriteRegister(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	    WriteRegister(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	    WriteRegister(0x12, 0x0000); // VREG1OUT voltage
	    WriteRegister(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
	
	    SysCtlDelay(200 * ulClockMS);

        //
        // Power on sequence for the ILI9320.
        //
        WriteRegister(0x10, 0x1690);
        WriteRegister(0x11, 0x0227);

        SysCtlDelay(50 * ulClockMS);

        WriteRegister(0x12, 0x000C);

        SysCtlDelay(50 * ulClockMS);

        WriteRegister(0x13, 0x0800);
        WriteRegister(0x29, 0x0011);
		WriteRegister(0x2B, 0x000B);
        SysCtlDelay(50 * ulClockMS);

	    //
	    // GRAM horizontal and vertical addresses
	    //
	    WriteRegister(0x20, 0x0000);
	    WriteRegister(0x21, 0x0000);
	
	    //
	    // Adjust the Gamma Curve
	    //
	    WriteRegister(0x30, 0x0000);
	    WriteRegister(0x31, 0x0106);
	    WriteRegister(0x32, 0x0000);
	    WriteRegister(0x35, 0x0204);
	    WriteRegister(0x36, 0x160A);
	    WriteRegister(0x37, 0x0707);
	    WriteRegister(0x38, 0x0106);
	    WriteRegister(0x39, 0x0707);
	    WriteRegister(0x3C, 0x0402);
	    WriteRegister(0x3D, 0x0C0F);
	
	    //
	    // Set GRAM area
	    //
	    WriteRegister(0x50, 0x0000); // Horizontal GRAM Start Address
	    WriteRegister(0x51, 0x00EF); // Horizontal GRAM End Address
	    WriteRegister(0x52, 0x0000); // Vertical GRAM Start Address
	    WriteRegister(0x53, 0x013F); // Vertical GRAM Start Address
	
	    //
	    // Driver output control 2, base image display control and vertical scroll
	    // control.
	    //
	    WriteRegister(0x60, 0x2700);
	    WriteRegister(0x61, 0x0001); // NDL,VLE, REV
	    WriteRegister(0x6A, 0x0000); // set scrolling line
	
	    //
	    // Partial Display Control
	    //
	    WriteRegister(0x80, 0x0000);
	    WriteRegister(0x81, 0x0000);
	    WriteRegister(0x82, 0x0000);
	    WriteRegister(0x83, 0x0000);
	    WriteRegister(0x84, 0x0000);
	    WriteRegister(0x85, 0x0000);
	
	    //
	    // Panel Control
	    //
	    WriteRegister(0x90, 0x0010);
	    WriteRegister(0x92, 0x0600);
	

	}

	else if (g_usController == 0x4535 || g_usController == 0x4531)
	{
		WriteRegister(0x15, 0x0030); 	   // Set the internal vcore voltage                                              
	    WriteRegister(0x9A, 0x0010); 	   // Start internal OSC.
	    WriteRegister(0x11, 0x0020);	   // set SS and SM bit
	    WriteRegister(0x10, 0x3428);	   // set 1 line inversion
	    WriteRegister(0x12, 0x0002);	   // set GRAM write direction and BGR=1. 
	    WriteRegister(0x13, 0x1038);	   // Resize register
	    SysCtlDelay(40 * ulClockMS);
	    WriteRegister(0x12, 0x0012);	   // set the back porch and front porch
	    SysCtlDelay(40 * ulClockMS); 
	    WriteRegister(0x10, 0x3420);	   // set non-display area refresh cycle ISC[3:0]
	    WriteRegister(0x13, 0x3045);	   // FMARK function
	    SysCtlDelay(70 * ulClockMS); 
	    WriteRegister(0x30, 0x0000);         // RGB interface setting
	    WriteRegister(0x31, 0x0402);	   // Frame marker Position
	    WriteRegister(0x32, 0x0307);         // RGB interface polarity
	    WriteRegister(0x33, 0x0304);         // SAP, BT[3:0], AP, DSTB, SLP, STB
	    WriteRegister(0x34, 0x0004);         // DC1[2:0], DC0[2:0], VC[2:0]
	    WriteRegister(0x35, 0x0401);         // VREG1OUT voltage
	    WriteRegister(0x36, 0x0707);         // VDV[4:0] for VCOM amplitude
	    WriteRegister(0x37, 0x0305);         // SAP, BT[3:0], AP, DSTB, SLP, STB
	    WriteRegister(0x38, 0x0610);         // DC1[2:0], DC0[2:0], VC[2:0]
	    WriteRegister(0x39, 0x0610);         // VREG1OUT voltage
	    WriteRegister(0x01, 0x0100);         // VDV[4:0] for VCOM amplitude
	    WriteRegister(0x02, 0x0300);         // VCM[4:0] for VCOMH
	    WriteRegister(0x03, 0x0030);         // GRAM horizontal Address
	    WriteRegister(0x08, 0x0808);         // GRAM Vertical Address
	    WriteRegister(0x0A, 0x0008);		
	    WriteRegister(0x60, 0x2700);	    // Gate Scan Line
	    WriteRegister(0x61, 0x0001);	    // NDL,VLE, REV
	    WriteRegister(0x90, 0x013E);
	    WriteRegister(0x92, 0x0100);
	    WriteRegister(0x93, 0x0100);
	    WriteRegister(0xA0, 0x3000);
	    WriteRegister(0xA3, 0x0010);  
	}
	
	else if (g_usController == 0x9325 || g_usController == 0x9320 || g_usController == 0x9328)
	{
	    //
	    // Internal timing configuration (common to both ILI9320, ILI9325 and
	    // ILI9328)
	    //
	    WriteRegister(0xE3, 0x3008);
	
	    if(g_usController != 0x9320)
	    {
	        //
	        // Set internal timing registers on the ILI9325/8 controller
	        //
	
	        WriteRegister(0xE7, 0x0012);
	        WriteRegister(0xEF, 0x1231);
	    }
	    else
	    {
	        //
	        // Enable the internal oscillator (ILI9320 only)
	        //
	        WriteRegister(0x00, 0x0001);
	    }
	
	    //
	    // Basic interface configuration (common to all controllers).
	    //
	    WriteRegister(0x01, 0x0100); // set SS and SM bit
	    WriteRegister(0x02, 0x0700); // set 1 line inversion
	    WriteRegister(0x03, 0x0030); // set GRAM write direction and BGR=0.
	    WriteRegister(0x04, 0x0000); // Resize register
	    WriteRegister(0x08, 0x0202); // set the back porch and front porch
	    WriteRegister(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
	    WriteRegister(0x0A, 0x0000); // FMARK function
	    WriteRegister(0x0C, 0x0001); // RGB interface setting
	    WriteRegister(0x0D, 0x0000); // Frame marker Position
	    WriteRegister(0x0F, 0x0000); // RGB interface polarity
	
	    //
	    // Power On sequence as provided by display manufacturer.
	    //
	    WriteRegister(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	    WriteRegister(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	    WriteRegister(0x12, 0x0000); // VREG1OUT voltage
	    WriteRegister(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
	
	    SysCtlDelay(200 * ulClockMS);
	
	    if(g_usController != 0x9320)
	    {
	        //
	        // Power on sequence for the ILI9325/8.
	        //
	        WriteRegister(0x10, 0x1690);
	        WriteRegister(0x11, 0x0227);
	
	        SysCtlDelay(50 * ulClockMS);
	
	        WriteRegister(0x12, 0x001A);
	
	        SysCtlDelay(50 * ulClockMS);
	
	        WriteRegister(0x13, 0x1800);
	        WriteRegister(0x29, 0x002A);
	        WriteRegister(0x2B, 0x000D);
	
	        SysCtlDelay(50 * ulClockMS);
	    }
	    else
	    {
	        //
	        // Power on sequence for the ILI9320.
	        //
	        WriteRegister(0x10, 0x17B0);
	        WriteRegister(0x11, 0x0137);
	
	        SysCtlDelay(50 * ulClockMS);
	
	        WriteRegister(0x12, 0x013C);
	
	        SysCtlDelay(50 * ulClockMS);
	
	        WriteRegister(0x13, 0x1900);
	        WriteRegister(0x29, 0x001A);
	
	        SysCtlDelay(50 * ulClockMS);	
	    }
    //
    // GRAM horizontal and vertical addresses
    //         
	WriteRegister(0x0020,0x0000);  
	WriteRegister(0x0021,0x0000); 

    //
    // Adjust the Gamma Curve
    //
    WriteRegister(0x30, (g_usController != 0x9320) ? 0x0007 : 0x0002);
    WriteRegister(0x31, (g_usController != 0x9320) ? 0x0605 : 0x0607);
    WriteRegister(0x32, (g_usController != 0x9320) ? 0x0106 : 0x0504);
    WriteRegister(0x35, 0x0206);
    WriteRegister(0x36, (g_usController != 0x9320) ? 0x0808: 0x0504);
    WriteRegister(0x37, (g_usController != 0x9320) ? 0x0007: 0x0606);
    WriteRegister(0x38, (g_usController != 0x9320) ? 0x0201: 0x0105);
    WriteRegister(0x39, 0x0007);
    WriteRegister(0x3C, (g_usController != 0x9320) ? 0x0602 : 0x0700);
    WriteRegister(0x3D, (g_usController != 0x9320) ? 0x0808 : 0x0700);

    //
    // Set GRAM area
    //
    WriteRegister(0x50, 0x0000); // Horizontal GRAM Start Address
    WriteRegister(0x51, 0x00EF); // Horizontal GRAM End Address
    WriteRegister(0x52, 0x0000); // Vertical GRAM Start Address
    WriteRegister(0x53, 0x013F); // Vertical GRAM Start Address

    //
    // Driver output control 2, base image display control and vertical scroll
    // control.
    //
    WriteRegister(0x60, (g_usController != 0x9320) ? 0xA700 : 0x2700);
    WriteRegister(0x61, 0x0001); // NDL,VLE, REV
    WriteRegister(0x6A, 0x0000); // set scrolling line

    //
    // Partial Display Control
    //
    WriteRegister(0x80, 0x0000);
    WriteRegister(0x81, 0x0000);
    WriteRegister(0x82, 0x0000);
    WriteRegister(0x83, 0x0000);
    WriteRegister(0x84, 0x0000);
    WriteRegister(0x85, 0x0000);

    //
    // Panel Control
    //
    WriteRegister(0x90, 0x0010);
    WriteRegister(0x92, 0x0000);
    WriteRegister(0x93, 0x0003);
    WriteRegister(0x95, 0x0110);
    WriteRegister(0x97, 0x0000);
    WriteRegister(0x98, 0x0000);
	}
	else
	{
		g_usController = ReadILI9338Register(0xD3);

		if (g_usController == 0x9338)
		{			 
			WriteCommand(0x11);//Exit Sleep
		    SysCtlDelay(100 * ulClockMS); 
			WriteCommand(0xCB); //AP[2:0]
			WriteData(0x01);
			WriteData(0xA4);
			WriteData(0x04);
			WriteData(0x6A);
	
			WriteCommand(0xC0); //Power control
			WriteData(0x26);//VRH[5:0]
			WriteData(0x01);//VC[2:0]
			WriteCommand(0xC1); //Power control
			WriteData(0x10);//SAP[2:0];BT[3:0]
			WriteCommand(0xC5); //VCM control
			WriteData(0x29);//0x24
			WriteData(0x3A);
			WriteCommand(0xC7); //
			WriteData(0xBD);//0x24
			WriteCommand(0x26); //Gamma curve selected
			WriteData(0x01);
			WriteCommand(0xE0); //Set Gamma
			WriteData(0x10);
			WriteData(0x10);
			WriteData(0x10);
			WriteData(0x08);
			WriteData(0x0E);
			WriteData(0x06);
			WriteData(0x42);
			WriteData(0x28);
			WriteData(0x36);
			WriteData(0x03);
			WriteData(0x0E);
			WriteData(0x04);
			WriteData(0x13);
			WriteData(0x0E);
			WriteData(0x0C);
			WriteCommand(0x36);
			WriteData(0x08);
			WriteCommand(0xE1); //Set Gamma
			WriteData(0x0C);
			WriteData(0x23);
			WriteData(0x26);
			WriteData(0x04);
			WriteData(0x0C);
			WriteData(0x04);
			WriteData(0x39);
			WriteData(0x24);
			WriteData(0x4B);
			WriteData(0x03);
			WriteData(0x0B);
			WriteData(0x0B);
			WriteData(0x33);
			WriteData(0x37);
			WriteData(0x0F);
			WriteCommand(0x3a);
			WriteData(0x55);
			WriteCommand(0x28);
			WriteCommand(0x29);
			WriteCommand(0x29);
		}
	}
	    
    //
    // Delay for 20ms, which is equivalent to two frames.
    //
    SysCtlDelay(20 * ulClockMS);	

    //
    // Clear the contents of the display buffer.
    //			  
    WriteCommand(0x22);
    for(ulCount = 0; ulCount < (320 * 240); ulCount++)
    {
        WriteData(0x0000);
    }

    //
    // Enable the image display.
    //
    WriteRegister(0x07, 0x0133);

    //
    // Delay for 20ms, which is equivalent to two frames.
    //
    SysCtlDelay(20 * ulClockMS);
}

//*****************************************************************************
//
//! Determines whether an LGDP4535 controller is present.
//!
//! This function queries the ID of the display controller in use and returns
//! it to the caller.  This driver supports LGDP4535.
//! These are very similar but the sense of the long display axis is reversed
//! in the Formike KWH028Q02-F03 using an LGDP4535 relative to the other two
//! supported displays and this information is needed by the touchscreen driver
//! to provide correct touch coordinate information.
//
//*****************************************************************************
unsigned short
Lcd240x320x16_8bitControllerIdGet(void)
{
    //
    // Determine which version of the display controller we are using.
    //
    g_usController = ReadRegister(0x00);

    return(g_usController);
}

//*****************************************************************************
//
//! Turns on the backlight.
//!
//! This function turns on the backlight on the display.
//!
//! \return None.
//
//*****************************************************************************
void
Lcd240x320x16_8bitBacklightOn(void)
{
    //
    // Assert the signal that turns on the backlight.
    //
    HWREG(LCD_BL_BASE + GPIO_O_DATA + (LCD_BL_PIN << 2)) = LCD_BL_PIN;
}

//*****************************************************************************
//
//! Turns off the backlight.
//!
//! This function turns off the backlight on the display.
//!
//! \return None.
//
//*****************************************************************************
void
Lcd240x320x16_8bitBacklightOff(void)
{
    //
    // Deassert the signal that turns on the backlight.
    //
    HWREG(LCD_BL_BASE + GPIO_O_DATA + (LCD_BL_PIN << 2)) = 0;
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535PixelDraw(void *pvDisplayData, long lX, long lY,
                                   unsigned long ulValue)
{
    //
    // Set the X address of the display cursor.
    //
    WriteCommand(0x20);
#ifdef PORTRAIT
    WriteData(lX);
#endif
#ifdef LANDSCAPE
    WriteData(239 - lY);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(lY);
#endif

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(0x21);
#ifdef PORTRAIT
    WriteData(lY);
#endif
#ifdef LANDSCAPE
    WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - lX);
#endif

    //
    // Write the pixel value.
    //
    WriteCommand(0x22);
    WriteData(ulValue);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535PixelDrawMultiple(void *pvDisplayData, long lX,
                                           long lY, long lX0, long lCount,
                                           long lBPP,
                                           const unsigned char *pucData,
                                           const unsigned char *pucPalette)
{
    unsigned long ulByte;

    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    WriteCommand(0x03);
#ifdef PORTRAIT
    WriteData(0x0030);
#endif
#ifdef LANDSCAPE
    WriteData(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(0x0018);
#endif

    //
    // Set the starting X address of the display cursor.
    //
    WriteCommand(0x20);
#ifdef PORTRAIT
    WriteData(lX);
#endif
#ifdef LANDSCAPE
    WriteData(239 - lY);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(lY);
#endif

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(0x21);
#ifdef PORTRAIT
    WriteData(lY);
#endif
#ifdef LANDSCAPE
    WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - lX);
#endif

    //
    // Write the data RAM write command.
    //
    WriteCommand(0x22);

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        //
        // The pixel data is in 1 bit per pixel format.
        //
        case 1:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount)
            {
                //
                // Get the next byte of image data.
                //
                ulByte = *pucData++;

                //
                // Loop through the pixels in this byte of image data.
                //
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    //
                    // Draw this pixel in the appropriate color.
                    //
                    WriteData(((unsigned long *)pucPalette)[(ulByte >>
                                                             (7 - lX0)) & 1]);
                }

                //
                // Start at the beginning of the next byte of image data.
                //
                lX0 = 0;
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // The pixel data is in 4 bit per pixel format.
        //
        case 4:
        {
            //
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            //
            switch(lX0 & 1)
            {
                case 0:
                    while(lCount)
                    {
                        //
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        //
                        ulByte = (*pucData >> 4) * 3;
                        ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                  0x00ffffff);

                        //
                        // Translate this palette entry and write it to the
                        // screen.
                        //
                        WriteData(DPYCOLORTRANSLATE(ulByte));

                        //
                        // Decrement the count of pixels to draw.
                        //
                        lCount--;

                        //
                        // See if there is another pixel to draw.
                        //
                        if(lCount)
                        {
                case 1:
                            //
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            //
                            ulByte = (*pucData++ & 15) * 3;
                            ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                      0x00ffffff);

                            //
                            // Translate this palette entry and write it to the
                            // screen.
                            //
                            WriteData(DPYCOLORTRANSLATE(ulByte));

                            //
                            // Decrement the count of pixels to draw.
                            //
                            lCount--;
                        }
                    }
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // The pixel data is in 8 bit per pixel format.
        //
        case 8:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount--)
            {
                //
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                //
                ulByte = *pucData++ * 3;
                ulByte = *(unsigned long *)(pucPalette + ulByte) & 0x00ffffff;

                //
                // Translate this palette entry and write it to the screen.
                //
                WriteData(DPYCOLORTRANSLATE(ulByte));
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // We are being passed data in the display's native format.  Merely
        // write it directly to the display.  This is a special case which is
        // not used by the graphics library but which is helpful to
        // applications which may want to handle, for example, JPEG images.
        //
        case 16:
        {
            unsigned short usByte;

            //
            // Loop while there are more pixels to draw.
            //
            while(lCount--)
            {
                //
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                //
                usByte = *((unsigned short *)pucData);
                pucData += 2;

                //
                // Translate this palette entry and write it to the screen.
                //
                WriteData(usByte);
            }
        }
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535LineDrawH(void *pvDisplayData, long lX1, long lX2,
                                   long lY, unsigned long ulValue)
{
    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    WriteCommand(0x03);
#ifdef PORTRAIT
    WriteData(0x0030);
#endif
#ifdef LANDSCAPE
    WriteData(0x0028);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(0x0000);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(0x0018);
#endif

    //
    // Set the starting X address of the display cursor.
    //
    WriteCommand(0x20);
#ifdef PORTRAIT
    WriteData(lX1);
#endif
#ifdef LANDSCAPE
    WriteData(239 - lY);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - lX1);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(lY);
#endif

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(0x21);
#ifdef PORTRAIT
    WriteData(lY);
#endif
#ifdef LANDSCAPE
    WriteData(lX1);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - lY);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - lX1);
#endif

    //
    // Write the data RAM write command.
    //
    WriteCommand(0x22);

    //
    // Loop through the pixels of this horizontal line.
    //
    while(lX1++ <= lX2)
    {
        //
        // Write the pixel value.
        //
        WriteData(ulValue);
    }
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535LineDrawV(void *pvDisplayData, long lX, long lY1,
                                   long lY2, unsigned long ulValue)
{
    //
    // Set the cursor increment to top to bottom, followed by left to right.
    //
    WriteCommand(0x03);
#ifdef PORTRAIT
    WriteData(0x0038);
#endif
#ifdef LANDSCAPE
    WriteData(0x0020);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(0x0008);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(0x0010);
#endif

    //
    // Set the X address of the display cursor.
    //
    WriteCommand(0x20);
#ifdef PORTRAIT
    WriteData(lX);
#endif
#ifdef LANDSCAPE
    WriteData(239 - lY1);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - lX);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(lY1);
#endif

    //
    // Set the starting Y address of the display cursor.
    //
    WriteCommand(0x21);
#ifdef PORTRAIT
    WriteData(lY1);
#endif
#ifdef LANDSCAPE
    WriteData(lX);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - lY1);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - lX);
#endif

    //
    // Write the data RAM write command.
    //
    WriteCommand(0x22);

    //
    // Loop through the pixels of this vertical line.
    //
    while(lY1++ <= lY2)
    {
        //
        // Write the pixel value.
        //
        WriteData(ulValue);
    }
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535RectFill(void *pvDisplayData, const tRectangle *pRect,
                                  unsigned long ulValue)
{
    long lCount;

    //
    // Write the X extents of the rectangle.
    //
    WriteCommand(0x50);
#ifdef PORTRAIT
    WriteData(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    WriteData(239 - pRect->sYMax);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - pRect->sXMax);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(pRect->sYMin);
#endif
    WriteCommand(0x51);
#ifdef PORTRAIT
    WriteData(pRect->sXMax);
#endif
#ifdef LANDSCAPE
    WriteData(239 - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(pRect->sYMax);
#endif

    //
    // Write the Y extents of the rectangle.
    //
    WriteCommand(0x52);
#ifdef PORTRAIT
    WriteData(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    WriteData(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - pRect->sXMax);
#endif
    WriteCommand(0x53);
#ifdef PORTRAIT
    WriteData(pRect->sYMax);
#endif
#ifdef LANDSCAPE
    WriteData(pRect->sXMax);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - pRect->sYMin);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - pRect->sXMin);
#endif

    //
    // Set the display cursor to the upper left of the rectangle.
    //
    WriteCommand(0x20);
#ifdef PORTRAIT
    WriteData(pRect->sXMin);
#endif
#ifdef LANDSCAPE
    WriteData(239 - pRect->sYMin);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(239 - pRect->sXMin);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(pRect->sYMin);
#endif
    WriteCommand(0x21);
#ifdef PORTRAIT
    WriteData(pRect->sYMin);
#endif
#ifdef LANDSCAPE
    WriteData(pRect->sXMin);
#endif
#ifdef PORTRAIT_FLIP
    WriteData(319 - pRect->sYMax);
#endif
#ifdef LANDSCAPE_FLIP
    WriteData(319 - pRect->sXMax);
#endif

    //
    // Write the data RAM write command.
    //
    WriteCommand(0x22);

    //
    // Loop through the pixels of this filled rectangle.
    //
    for(lCount = ((pRect->sXMax - pRect->sXMin + 1) *
                  (pRect->sYMax - pRect->sYMin + 1)); lCount >= 0; lCount--)
    {
        //
        // Write the pixel value.
        //
        WriteData(ulValue);
    }

    //
    // Reset the X extents to the entire screen.
    //
    WriteCommand(0x50);
    WriteData(0x0000);
    WriteCommand(0x51);
    WriteData(0x00ef);

    //
    // Reset the Y extents to the entire screen.
    //
    WriteCommand(0x52);
    WriteData(0x0000);
    WriteCommand(0x53);
    WriteData(0x013f);
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static unsigned long
Formike240x320x16_LGDP4535ColorTranslate(void *pvDisplayData,
                                        unsigned long ulValue)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    return(DPYCOLORTRANSLATE(ulValue));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  For the ILI932x
//! driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void
Formike240x320x16_LGDP4535Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
}

//*****************************************************************************
//
//! The graphics library display structure that describes the driver for the
//! F02, F03 or F05 variants of the Formike Electronic KWH028Q02 TFT panel with
//! ILI932x controllers.
//
//*****************************************************************************
const tDisplay g_sLcd240x320x16_8bit =
{
    sizeof(tDisplay),
    0,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    240,
    320,
#else
    320,
    240,
#endif
    Formike240x320x16_LGDP4535PixelDraw,
    Formike240x320x16_LGDP4535PixelDrawMultiple,
    Formike240x320x16_LGDP4535LineDrawH,
    Formike240x320x16_LGDP4535LineDrawV,
    Formike240x320x16_LGDP4535RectFill,
    Formike240x320x16_LGDP4535ColorTranslate,
    Formike240x320x16_LGDP4535Flush
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
