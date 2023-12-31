/*******************************************************************************
** File  Name  : HX8352A0240x400x8.c                                          **
** Description : Display driver for the HX8352A TFT LCD display with an		  **
**               SPFD5420 controller. This version assumes an 8-bit interfac  **
**               between the micro and display)                               **
**                                                                            **
*******************************************************************************/

#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"

#include "gpio.h"
#include "interrupt.h"
#include "sysctl.h"
#include "timer.h"

#include "grlib.h"
#include "HX8352A240x400x8.h"
#include "HIMAX8352.h"
//#include "set_pinout.h"



static void	InitGPIOLCDInterface(unsigned long ulClockMS);
static void WriteDataGPIO(unsigned short usData);
static void WriteCommandGPIO(unsigned short usData);
//*****************************************************************************
// This driver operates in four different screen orientations.  They are:
// * Portrait - The screen is taller than it is wide, and the flex connector is
//              on the bottom of the display.  This is selected by defining
//              PORTRAIT.
// * Landscape - The screen is wider than it is tall, and the flex connector is
//               on the left of the display.  This is selected by defining
//               LANDSCAPE.
// * Portrait flip - The screen is taller than it is wide, and the flex
//                   connector is on the top of the display.  This is
//                   selected by defining PORTRAIT_FLIP.
// * Landscape flip - The screen is wider than it is tall, and the flex
//                    connector is on the right of the display.  This is
//                    selected by defining LANDSCAPE_FLIP.
// These can also be imagined in terms of screen rotation; if portrait mode is
// 0 degrees of screen rotation, landscape is 90 degrees of counter-clockwise
// rotation, portrait flip is 180 degrees of rotation, and landscape flip is
// 270 degress of counter-clockwise rotation.
// If no screen orientation is selected, "landscape flip" mode will be used.
//*****************************************************************************
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define PORTRAIT
#endif

//*****************************************************************************
// Various definitions controlling coordinate space mapping and drawing
// direction in the four supported orientations.
//*****************************************************************************
#ifdef PORTRAIT
#define HORIZ_DIRECTION 	0x28
#define VERT_DIRECTION 		0x20
#define MAPPED_X(x, y) 		(399 - (y))
#define MAPPED_Y(x, y) 		(x)
#endif
#ifdef LANDSCAPE
#define HORIZ_DIRECTION 	0x00
#define VERT_DIRECTION  	0x08
#define MAPPED_X(x, y) 		(399 - (x))
#define MAPPED_Y(x, y) 		(239 - (y))
#endif
#ifdef PORTRAIT_FLIP
#define HORIZ_DIRECTION 	0x18
#define VERT_DIRECTION 		0x10
#define MAPPED_X(x, y) 		(y)
#define MAPPED_Y(x, y) 		(239 - (x))
#endif
#ifdef LANDSCAPE_FLIP
#define HORIZ_DIRECTION 	0x30
#define VERT_DIRECTION  	0x38
#define MAPPED_X(x, y) 		(x)
#define MAPPED_Y(x, y) 		(y)
#endif

//*****************************************************************************
// Defines for the pins that are used to communicate with the SPFD5420.
//*****************************************************************************
#define LCD_DATAH_PINS          0xFF
#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOD
#define LCD_DATAH_BASE          GPIO_PORTD_BASE

// LCD control line GPIO definitions.
#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_CS_BASE             GPIO_PORTE_BASE
#define LCD_CS_PIN              GPIO_PIN_1

#define LCD_FMARK_PERIPH        SYSCTL_PERIPH_GPIOE
#define LCD_FMARK_BASE          GPIO_PORTE_BASE
#define LCD_FMARK_PIN           GPIO_PIN_0

#define LCD_RST_PERIPH          SYSCTL_PERIPH_GPIOC
#define LCD_RST_BASE            GPIO_PORTC_BASE
#define LCD_RST_PIN             GPIO_PIN_7

#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOH
#define LCD_RD_BASE             GPIO_PORTH_BASE
#define LCD_RD_PIN              GPIO_PIN_2

#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOH
#define LCD_WR_BASE             GPIO_PORTH_BASE
#define LCD_WR_PIN              GPIO_PIN_0

#define LCD_RS_PERIPH           SYSCTL_PERIPH_GPIOH
#define LCD_RS_BASE             GPIO_PORTH_BASE
#define LCD_RS_PIN              GPIO_PIN_1
//*****************************************************************************
// Backlight control GPIO used with the Flash/SRAM/LCD daughter board.
//*****************************************************************************
#define LCD_LEDA_PERIPH         SYSCTL_PERIPH_GPIOC
#define LCD_LEDA_BASE      		GPIO_PORTC_BASE
#define LCD_LEDA_PIN       		GPIO_PIN_6
											  
#define LCD_LEDK1_PERIPH        SYSCTL_PERIPH_GPIOC
#define LCD_LEDK1_BASE      	GPIO_PORTC_BASE
#define LCD_LEDK1_PIN       	GPIO_PIN_5

#define LCD_LEDK2_PERIPH        SYSCTL_PERIPH_GPIOC
#define LCD_LEDK2_BASE      	GPIO_PORTC_BASE
#define LCD_LEDK2_PIN       	GPIO_PIN_4

#define LCD_LEDK3_PERIPH        SYSCTL_PERIPH_GPIOA
#define LCD_LEDK3_BASE      	GPIO_PORTA_BASE
#define LCD_LEDK3_PIN       	GPIO_PIN_2

#define LCD_LEDK4_PERIPH        SYSCTL_PERIPH_GPIOA
#define LCD_LEDK4_BASE      	GPIO_PORTA_BASE
#define LCD_LEDK4_PIN       	GPIO_PIN_3

//*****************************************************************************
// Macro used to set the LCD data bus in preparation for writing a byte to the
// device.
//*****************************************************************************
#define SET_LCD_DATA(ucByte)                                                  \
{                                                                             \
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucByte);   \
}

#define SET_LCD_WR(ucByte)												\
{																		\
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = (ucByte);   	\
}

#define SET_LCD_RD(ucByte)												\
{																		\
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = (ucByte);   	\
}			

#define SET_LCD_RS(ucByte)												\
{																		\
    HWREG(LCD_RS_BASE + GPIO_O_DATA + (LCD_RS_PIN << 2)) = (ucByte);   	\
}

#define SET_LCD_CS(ucByte)												\
{																		\
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = (ucByte);   	\
}

#define SET_LCD_RST(ucByte)												\
{																		\
    HWREG(LCD_RST_BASE + GPIO_O_DATA + (LCD_RST_PIN << 2)) = (ucByte);   	\
}
		

//*****************************************************************************
// Function pointer types for low level LCD controller access functions.
//*****************************************************************************
typedef void (*pfnWriteData)(unsigned short usData);
typedef void (*pfnWriteCommand)(unsigned short ucData);

//*****************************************************************************
// Function pointers for low level LCD controller access functions.
//*****************************************************************************
pfnWriteData WriteData = WriteDataGPIO;
pfnWriteCommand WriteCommand = WriteCommandGPIO;	
	
//*****************************************************************************
// The dimensions of the LCD panel.
//*****************************************************************************
#define LCD_VERTICAL_MAX 	240
#define LCD_HORIZONTAL_MAX 	400

/******************************************************************************
** Function Name : HX8352240x400x8_HX8352Init                                **
** Description   : This function initializes the HIMAX8352 display controller**
**                 on the panel, preparing it to display data.               **
** Arguments     : 														     **
** Output        :                                                           **
** Return        :                                                           **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
unsigned short Himax8352_Start[6][2] =
{
	{HIMAX8352A_TEST_MOD,0x0002},
	{HIMAX8352A_VDDD_CTL,0x0003},
	{HIMAX8352A_VGS_CTL1,0x0000},
	{HIMAX8352A_VGS_CTL2,0x0013},
	{HIMAX8352A_PWM_CTL0,0x0001},
	{HIMAX8352A_TEST_MOD,0x0000}
};
unsigned short Himax8352_Gamma[12][2] = 
{
	{HIMAX8352A_GAMMA_CTL1, 0x00C4},
	{HIMAX8352A_GAMMA_CTL2, 0x0044},
	{HIMAX8352A_GAMMA_CTL3, 0x0022},
	{HIMAX8352A_GAMMA_CTL4, 0x0057},
	{HIMAX8352A_GAMMA_CTL5, 0x0003},
	{HIMAX8352A_GAMMA_CTL6, 0x0047},
	{HIMAX8352A_GAMMA_CTL7, 0x0002},
	{HIMAX8352A_GAMMA_CTL8, 0x0055},
	{HIMAX8352A_GAMMA_CTL9, 0x0006},
	{HIMAX8352A_GAMMA_CTL10,0x004C},
	{HIMAX8352A_GAMMA_CTL11,0x0006},
	{HIMAX8352A_GAMMA_CTL12,0x008C},
};
unsigned short Himax8352_RGBLUT[64]=
{
	0x0000, 0x0003, 0x000A, 0x000F, 0x0013, 0x0016, 0x0019, 0x001C,	//--0		
	0x001E, 0x001F, 0x0025, 0x002A, 0x0030, 0x0035, 0x0039, 0x003D,	//--8
	0x0041, 0x0045, 0x0048, 0x004C, 0x004F, 0x0053, 0x0058, 0x005D,	//--16
	0x0061, 0x0066, 0x006A, 0x006E, 0x0072, 0x0076, 0x007A, 0x007E,	//--24
	0x0082, 0x0085, 0x0089, 0x008D, 0x0090, 0x0094, 0x0097, 0x009A,	//--32
	0x009D, 0x00A2, 0x00A5, 0x00A9, 0x00AC, 0x00B0, 0x00B4, 0x00B8,	//--40
	0x00BC, 0x00C0, 0x00C3, 0x00C8, 0x00CC, 0x00D2, 0x00D6, 0x00DC,	//--48
	0x00DF, 0x00E2, 0x00E5, 0x00E8, 0x00EC, 0x00EF, 0x00F4, 0x00FF,	//--56
};
void HX8352240x400x8_HX8352Init(void)
{
    //unsigned char ucP;
	long i = 0;
	long  ulClockMS;
	
	// Get the current processor clock frequency.
    ulClockMS = SysCtlClockGet() / (3 * 1000);


	InitGPIOLCDInterface(ulClockMS);
	//********************** Start initial squence ******************
	for (i = 0; i < 6; i++)
	{
		WriteCommand(Himax8352_Start[i][0]);
		WriteData(Himax8352_Start[i][1]);
	}
	
    SysCtlDelay(5 * ulClockMS);
	//********************** Gamma Setting ******************
	for (i = 0; i < 12; i++)
	{
		WriteCommand(Himax8352_Gamma[i][0]);
		WriteData(Himax8352_Gamma[i][1]);
	}
	//********************** Power Supply Setting ******************
	WriteCommand(HIMAX8352A_OSC_CTRL1); 	WriteData(0x0091);
	WriteCommand(HIMAX8352A_DSP_CTL1); 		WriteData(0x0001);
	WriteCommand(HIMAX8352A_CYCLE_CTL1); 	WriteData(0x00F9);
	SysCtlDelay(20 * ulClockMS);
	WriteCommand(HIMAX8352A_OSC_CTRL2); 	WriteData(0x003A); 

   	WriteCommand(HIMAX8352A_POWER_CTL3); 	WriteData(0x0011);  
   	WriteCommand(HIMAX8352A_TEST_MOD); 		WriteData(0x0002);  
	WriteCommand(0x008A); 					WriteData(0x0006);  
	WriteCommand(HIMAX8352A_TEST_MOD); 		WriteData(0x0000);  
	WriteCommand(HIMAX8352A_POWER_CTL2); 	WriteData(0x0011);  
	
	WriteCommand(HIMAX8352A_POWER_CTL4); 	WriteData(0x000A);  	
	WriteCommand(HIMAX8352A_VCOM_CTL); 		WriteData(0x0060);  	
	SysCtlDelay(30 * ulClockMS);

	WriteCommand(HIMAX8352A_POWER_CTL1); 	WriteData(0x000A);  	
	WriteCommand(HIMAX8352A_POWER_CTL1); 	WriteData(0x001A);  	
	SysCtlDelay(50 * ulClockMS);
	WriteCommand(HIMAX8352A_POWER_CTL1); 	WriteData(0x0012);  	
	SysCtlDelay(50 * ulClockMS);
	WriteCommand(HIMAX8352A_POWER_CTL6); 	WriteData(0x0031);  	
	SysCtlDelay(100 * ulClockMS);

	//********************** DUG FUNCTION ON ******************
	WriteCommand(HIMAX8352A_IP_CTL); 		WriteData(0x0001);  	
	
	//********************** RGB LUT ******************
	WriteCommand(HIMAX8352A_DGC_LUT_W);
	for (i = 0; i < 64; i++)
	{
		WriteData(Himax8352_RGBLUT[i]); 	
	}

	//********************** Display On Setting  ******************
	WriteCommand(0x003C); WriteData(0x00C0);  	
	WriteCommand(0x003D); WriteData(0x001C);  	
	WriteCommand(0x0034); WriteData(0x0038);  	
	WriteCommand(0x0035); WriteData(0x0038);  	
	WriteCommand(0x0024); WriteData(0x0038);  	
	SysCtlDelay(50 * ulClockMS);
	WriteCommand(0x0024); WriteData(0x003C);  	
	WriteCommand(0x0016); WriteData(0x001C);  	

    WriteCommand(0x0001); WriteData(0x0006);  	
	WriteCommand(0x0055); WriteData(0x0000);  	
     
	WriteCommand(0x0002); WriteData(0x0000);  	
	WriteCommand(0x0003); WriteData(0x0000);  	
    WriteCommand(0x0004); WriteData(0x0000);  	
	WriteCommand(0x0005); WriteData(0x00EF);  	
    WriteCommand(0x0006); WriteData(0x0000);  	
	WriteCommand(0x0007); WriteData(0x0000);  	
    WriteCommand(0x0008); WriteData(0x0001);  	
	WriteCommand(0x0009); WriteData(0x008F);  	
	WriteCommand(0x0022);
	SysCtlDelay(10 * ulClockMS);

}

/******************************************************************************
** Function Name : InitGPIOLCDInterface                                      **
** Description   : Initializes the pins required for the GPIO-based LCD      **
**                 interface.                                                **
**                 This function configures the GPIO pins used to control the**
**                 LCD display when the basic GPIO interface is in use.  On  **
**                 exit, the LCD controller has been reset and is ready to   **
**                 receive command and data writes. 					     **
** Arguments     : 														     **
** Output        :                                                           **
** Return        :                                                           **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void	InitGPIOLCDInterface(unsigned long ulClockMS)
{
    // Configure the pins that connect to the LCD as GPIO outputs.
    GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_FMARK_BASE, LCD_FMARK_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_FMARK_BASE, LCD_FMARK_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    GPIODirModeSet(LCD_RS_BASE, LCD_RS_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RS_BASE, LCD_RS_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    // Set the LCD control pins to their default values.  This also asserts the
    // LCD reset signal.
    GPIOPinWrite(LCD_DATAH_BASE, LCD_DATAH_PINS, 0x00);
    GPIOPinWrite(LCD_RS_BASE, LCD_RS_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);

    // Delay for 1ms.
    SysCtlDelay(ulClockMS);

    // Deassert the LCD reset signal.
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0);

    // Delay for 1ms while the LCD comes out of reset.
    SysCtlDelay(ulClockMS);
    GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);
	SysCtlDelay(ulClockMS);
}
/******************************************************************************
** Function Name : WriteCommandGPIO                                          **
** Description   : Writes a command to the HIMAX8352.  This function         **
**                 implements the basic GPIO interface to the LCD display    **
** Arguments     : 														     **
** Output        :                                                           **
** Return        :                                                           **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void WriteCommandGPIO(unsigned short usData)
{
    // Write the most significant byte of the data to the bus.  This is always
    // 0 since commands are no more than 8 bits.
	SET_LCD_DATA(0);

    SET_LCD_RD(LCD_RD_PIN);	// Read Signal
	SET_LCD_RS(0);			// Data/Register Select Signal

	SET_LCD_CS(0);
	SET_LCD_DATA(usData >> 8);
	SET_LCD_WR(0);
	SET_LCD_WR(0);
	SET_LCD_WR(LCD_WR_PIN);
	SET_LCD_CS(LCD_CS_PIN);

 	SET_LCD_CS(0);
	SET_LCD_DATA(usData);
	SET_LCD_WR(0);
	SET_LCD_WR(0);
	SET_LCD_WR(LCD_WR_PIN);
	SET_LCD_CS(LCD_CS_PIN);
}

/******************************************************************************
** Function Name : WriteCommandGPIO                                          **
** Description   : Writes a data to the HIMAX8352.  This function            **
**                 implements the basic GPIO interface to the LCD display    **
** Arguments     : 														     **
** Output        :                                                           **
** Return        :                                                           **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void WriteDataGPIO(unsigned short usData)
{
    SET_LCD_RD(LCD_RD_PIN);		// Read Signal
	SET_LCD_RS(LCD_RS_PIN);		// Data/Register Select Signal
	
	SET_LCD_CS(0);
	SET_LCD_DATA(usData >> 8);
	SET_LCD_WR(0);
	SET_LCD_WR(0);
	SET_LCD_WR(LCD_WR_PIN);
	SET_LCD_CS(LCD_CS_PIN);

	SET_LCD_CS(0);
	SET_LCD_DATA(usData);
	SET_LCD_WR(0);
	SET_LCD_WR(0);
	SET_LCD_WR(LCD_WR_PIN);
	SET_LCD_CS(LCD_CS_PIN);
}

/******************************************************************************
** Function Name : WriteCommandGPIO                                          **
** Description   : Draws a pixel on the screen.                              **
**                 This function sets the given pixel to a particular color. **
**                 The coordinates of the pixel are assumed to be within the **
**                 extents of the display.                                   **
** Arguments     : pvDisplayData  - is a pointer to the driver-specific data **
**                                  for this display driver.                 **
**                 lX - is the X coordinate of the pixel.		             **
**				   lY - is the Y coordinate of the pixel.                    **
**				   ulValue - is the color of the pixel.	                     **
** Output        :                                                           **
** Return        : None                                                      **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void HX8352A_PixelDraw(void *pvDisplayData, long lX, long lY,
                                   unsigned long ulValue)
{
    //
    // Set the X address of the display cursor.
    //
/*    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(lX, lY));

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(lX, lY));

    //
    // Write the pixel value.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);
    WriteData(ulValue);*/
}

/******************************************************************************
** Function Name : HX8352AGetIntoSleep                                          **
** Description   : Draws a pixel on the screen.                              **
**                 This function sets the given pixel to a particular color. **
**                 The coordinates of the pixel are assumed to be within the **
**                 extents of the display.                                   **
** Arguments     : pvDisplayData  - is a pointer to the driver-specific data **
**                                  for this display driver.                 **
**                 lX - is the X coordinate of the pixel.		             **
**				   lY - is the Y coordinate of the pixel.                    **
**				   ulValue - is the color of the pixel.	                     **
** Output        :                                                           **
** Return        : None                                                      **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void HX8352AGetIntoSleep(void)
{
//	long i = 0;
	long  ulClockMS;
	
	// Get the current processor clock frequency.
    ulClockMS = SysCtlClockGet() / (3 * 1000);

	// Display OFF
	WriteCommand(0x0024); WriteData(0x0038);
	SysCtlDelay(40*ulClockMS);
 	WriteCommand(0x0024); WriteData(0x0028);
 	SysCtlDelay(40*ulClockMS);
 	WriteCommand(0x0024); WriteData(0x0000);

	// Power OFF
	WriteCommand(0x001E); WriteData(0x0014);
	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x0019); WriteData(0x0002);
 	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x0019); WriteData(0x000A);
 	SysCtlDelay(10*ulClockMS);
	WriteCommand(0x001B); WriteData(0x0040);
 	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x003C); WriteData(0x0000);
 	SysCtlDelay(10*ulClockMS);

	// Get into STB mode
 	WriteCommand(0x0019); WriteData(0x000B);
 	SysCtlDelay(10*ulClockMS);

	// Stop Oscillation
 	WriteCommand(0x0017); WriteData(0x0090);
}

/******************************************************************************
** Function Name : HX8352AGetIntoSleep                                          **
** Description   : Draws a pixel on the screen.                              **
**                 This function sets the given pixel to a particular color. **
**                 The coordinates of the pixel are assumed to be within the **
**                 extents of the display.                                   **
** Arguments     : pvDisplayData  - is a pointer to the driver-specific data **
**                                  for this display driver.                 **
**                 lX - is the X coordinate of the pixel.		             **
**				   lY - is the Y coordinate of the pixel.                    **
**				   ulValue - is the color of the pixel.	                     **
** Output        :                                                           **
** Return        : None                                                      **
===============================================================================
** Date          :                                                           **
** Author        :                                                           **
******************************************************************************/
static void HX8352AGetOutSleep(void)
{
//	long i = 0;
	long  ulClockMS;
	
	// Get the current processor clock frequency.
    ulClockMS = SysCtlClockGet() / (3 * 1000);

	// Start Oscillation
	WriteCommand(0x0017); WriteData(0x0091);
	SysCtlDelay(10*ulClockMS);
	// Exit STB mode
 	WriteCommand(0x0019); WriteData(0x000A);

	// Power Supply Setting
 	WriteCommand(0x001B); WriteData(0x0011);
 	WriteCommand(0x001A); WriteData(0x0011);
 	WriteCommand(0x001C); WriteData(0x000A);
 	WriteCommand(0x001F); WriteData(0x0058);
	SysCtlDelay(20 * ulClockMS);

 	WriteCommand(0x0019); WriteData(0x000A);
	WriteCommand(0x001F); WriteData(0x001A);
	SysCtlDelay(40 * ulClockMS);

	WriteCommand(0x0019); WriteData(0x0012);
	SysCtlDelay(40 * ulClockMS);
	WriteCommand(0x001E); WriteData(0x002E);
	SysCtlDelay(100 * ulClockMS);
	
	// Display On Setting
	WriteCommand(0x003C); WriteData(0x00C0);
	WriteCommand(0x003D); WriteData(0x001C);
	WriteCommand(0x0034); WriteData(0x0038);
	WriteCommand(0x0035); WriteData(0x0038);
	WriteCommand(0x0024); WriteData(0x0038);   // GON = 1, DTE = 1, D =10,
	SysCtlDelay(40 * ulClockMS);

	WriteCommand(0x0024); WriteData(0x003C);	// GON =1 DTE =1 D=11






	// Power OFF
	WriteCommand(0x001E); WriteData(0x0014);
	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x0019); WriteData(0x0002);
 	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x0019); WriteData(0x000A);
 	SysCtlDelay(10*ulClockMS);
	WriteCommand(0x001B); WriteData(0x0040);
 	SysCtlDelay(10*ulClockMS);
 	WriteCommand(0x003C); WriteData(0x0000);
 	SysCtlDelay(10*ulClockMS);

	// Get into STB mode
 	WriteCommand(0x0019); WriteData(0x000B);
 	SysCtlDelay(10*ulClockMS);

	// Stop Oscillation
 	WriteCommand(0x0017); WriteData(0x0090);
}

