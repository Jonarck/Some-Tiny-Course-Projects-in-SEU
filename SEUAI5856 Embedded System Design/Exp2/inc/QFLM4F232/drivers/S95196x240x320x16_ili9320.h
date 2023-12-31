/************************************************************************************
** File  Name  : S95196x240x320x16_ili9320.h                                    **
** Description : Prototypes for the S95196 display driver with an ILI9320          **
**               controller.                                                       **
**               This is part of revision 8034 of the EK-LM4F232 Firmware Package. **
=====================================================================================
** Author Name : Triton.zhang                                                      **
** Data        : 2011-10-20                                                        **
** Notes       :                                                                   **
************************************************************************************/
#ifndef __S95196X240X320X16_ILI9320_H__
#define __S95196X240X320X16_ILI9320_H__

//===================================================================================
// The dimensions of the LCD panel.
//===================================================================================
#define LCD_VERTICAL_MAX      240
#define LCD_HORIZONTAL_MAX    320

//===================================================================================
// Defines for the pins that are used to communicate with the ILI932x.
//===================================================================================
#define LCD_DATAH_PINS          0xFF		
#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOA
#define LCD_DATAH_BASE          GPIO_PORTA_BASE

#define LCD_RST_PERIPH          SYSCTL_PERIPH_GPIOC
#define LCD_RST_BASE            GPIO_PORTC_BASE
#define LCD_RST_PIN             GPIO_PIN_6

#define LCD_LEDK_PERIPH        SYSCTL_PERIPH_GPIOC
#define LCD_LEDK_BASE          GPIO_PORTC_BASE
#define LCD_LEDK_PIN           GPIO_PIN_7

#define LCD_DATAL_PINS          0xFF
#define LCD_DATAL_PERIPH        SYSCTL_PERIPH_GPIOD
#define LCD_DATAL_BASE          GPIO_PORTD_BASE

#define LCD_RS_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_RS_BASE             GPIO_PORTE_BASE
#define LCD_RS_PIN              GPIO_PIN_6

#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_RD_BASE             GPIO_PORTE_BASE
#define LCD_RD_PIN              GPIO_PIN_7

#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOG
#define LCD_CS_BASE             GPIO_PORTG_BASE
#define	LCD_CS_PIN 			    GPIO_PIN_6

#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOG
#define LCD_WR_BASE             GPIO_PORTG_BASE
#define LCD_WR_PIN              GPIO_PIN_7


//===================================================================================
// Macro used to set the LCD data bus in preparation for writing a byte to the device.
//===================================================================================
//#define SET_LCD_DATAH(ucByte)                                                  \
//{                                                                             \
//    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucByte);   \
//}
//#define SET_LCD_DATAL(ucByte)                                                  \
//{                                                                             \
//    HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (ucByte);   \
//}

//#define	LCD_WRITE_DATA_16BIT(unData)\
//{\
//	SET_LCD_DATAH(unData >> 8);	\
//	SET_LCD_DATAL(unData & 0xFF);	\
//}

//#define	LCD_READ_DATA_16BIT()\
	//((HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) << 8) + \
    //(HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (0xff << 2))))			\

#define	LCD_SET_CS(ucByte) \
{\
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_WR(ucByte)	\
{								\
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_RS(ucByte) \
{ \
    HWREG(LCD_RS_BASE + GPIO_O_DATA + (LCD_RS_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_RD(ucByte) \
{ \
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = ucByte;		\
}

#define LCD_SET_LED1(ucByte)   \
{                                                                             \
    HWREG(LCD_LED_K1_BASE + GPIO_O_DATA + (LCD_LED_K1_PIN << 2)) = ucByte;   \
}

#define LCD_SET_LED2(ucByte)   \
{                                                                             \
    HWREG(LCD_LED_K2_BASE + GPIO_O_DATA + (LCD_LED_K2_PIN << 2)) = ucByte;   \
}

#define LCD_SET_LED3(ucByte)  \
{                                                                             \
    HWREG(LCD_LED_K3_BASE + GPIO_O_DATA + (LCD_LED_K3_PIN << 2)) = ucByte;   \
}

#define LCD_SET_LED4(ucByte)   \
{                                                                             \
    HWREG(LCD_LED_K4_BASE + GPIO_O_DATA + (LCD_LED_K4_PIN << 2)) = ucByte;   \
}

#define LCD_SET_LED_ON() \
{						\
	LCD_SET_LED1(LCD_LED_K1_PIN);\
	LCD_SET_LED2(LCD_LED_K2_PIN);\
	LCD_SET_LED3(LCD_LED_K3_PIN);\
	LCD_SET_LED4(LCD_LED_K4_PIN);\
}

#define	 LCD_SET_LED_OFF()\
{	\
	LCD_SET_LED1(LCD_LED_K1_PIN);\
	LCD_SET_LED2(LCD_LED_K2_PIN);\
	LCD_SET_LED3(LCD_LED_K3_PIN);\
	LCD_SET_LED4(LCD_LED_K4_PIN);\
}


//===================================================================================
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
//===================================================================================
#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00ff0000) >> 19) |               \
                                 ((((c) & 0x0000ff00) >> 5) & 0x000007e0) | \
                                 ((((c) & 0x000000ff) << 8) & 0x0000f800))

//===================================================================================
// Prototypes for the globals exported by this driver.
//===================================================================================
extern void S95196x240x320x16_ILI9320Init(void);
extern void S95196x240x320x16_ILI9320BacklightOn(void);
extern unsigned short S95196x240x320x16_ILI9320ControllerIdGet(void);
extern void S95196x240x320x16_ILI9320BacklightOff(void);
extern const tDisplay g_sS95196x240x320x16_ILI9320;

//===================================================================================
// The dimensions of the LCD panel.
//===================================================================================

#endif // __S95196X240X320X16_ILI9320_H__
