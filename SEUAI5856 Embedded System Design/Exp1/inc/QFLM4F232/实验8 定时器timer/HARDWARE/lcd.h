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
#ifndef __LCD_H__
#define __LCD_H__

 //定义LCD的尺寸	
#define LCD_W 479
#define LCD_H 271

//===================================================================================
// The dimensions of the LCD panel.
//===================================================================================
#define LCD_VERTICAL_MAX      240
#define LCD_HORIZONTAL_MAX    320


//LCD重要参数集
typedef struct  
{	
	unsigned char	bus16;				    
	unsigned int width;			//LCD 宽度
	unsigned int height;			//LCD 高度
	unsigned int id;				//LCD ID
	unsigned char	wramcmd;		//开始写gram指令
	unsigned char  setxcmd;		//设置x坐标指令
	unsigned char  setycmd;		//设置y坐标指令	 
}_lcd_dev; 

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

#define   HDP	479
#define   HT	531
#define   HPS	43
#define   LPS	8
#define   HPW	10
#define   VDP	271
#define   VT	288
#define   VPS	12
#define   FPS	4
#define   VPW	10

/***********************************************************************************
设置颜色宏定义
***********************************************************************************/
#define   Black        0x0000                    // 黑色： 0, 0, 0              
#define   Navy         0x000F                    // 深蓝色： 0, 0, 128          
#define   Dgreen       0x03E0                    // 深绿色： 0, 128, 0          
#define   Dcyan        0x03EF                    // 深青色： 0, 128, 128        
#define   Marooon       0x7800                    // 深红色：128, 0, 0           
#define   Purple       0x780F                    // 紫色： 128, 0, 128          
#define   Olive        0x7BE0                    // 橄榄绿：128, 128, 0         
#define   Lgray        0xC618                    // 灰白色：192, 192, 192        
#define   Dgray        0x7BEF                    // 深灰色：128, 128, 128        
#define   Blue         0x001F                    // 蓝色： 0, 0, 255            
#define   Green        0x07E0                 	 // 绿色： 0, 255, 0             
#define   Cyan         0x07FF                    // 青色： 0, 255, 255           
#define   Red          0xF800                    // 红色： 255, 0, 0             
#define   Magenta      0xF81F                    // 品红： 255, 0, 255          
#define   Yellow       0xFFE0                    // 黄色： 255, 255, 0           
#define   White        0xFFFF                    // 白色： 255, 255, 255   



//#define g_sLcdDisplay	g_slcd_320x240_ili932x
//===================================================================================
// Defines for the pins that are used to communicate with the ILI932x.
//===================================================================================
#define LCD_DATAH_PINS          0xFF		
#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOA
#define LCD_DATAH_BASE          GPIO_PORTA_BASE

#define LCD_RST_PERIPH          SYSCTL_PERIPH_GPIOF
#define LCD_RST_BASE            GPIO_PORTF_BASE
#define LCD_RST_PIN             GPIO_PIN_6

#define LCD_LEDK_PERIPH        SYSCTL_PERIPH_GPIOJ
#define LCD_LEDK_BASE          GPIO_PORTJ_BASE
#define LCD_LEDK_PIN           GPIO_PIN_2

#define LCD_DATAL_PINS          0xFF
#define LCD_DATAL_PERIPH        SYSCTL_PERIPH_GPIOD
#define LCD_DATAL_BASE          GPIO_PORTD_BASE

#define LCD_RS_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_RS_BASE             GPIO_PORTE_BASE
#define LCD_RS_PIN              GPIO_PIN_2

#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_RD_BASE             GPIO_PORTE_BASE
#define LCD_RD_PIN              GPIO_PIN_0

#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_CS_BASE             GPIO_PORTE_BASE
#define	LCD_CS_PIN 			        GPIO_PIN_3

#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOE
#define LCD_WR_BASE             GPIO_PORTE_BASE
#define LCD_WR_PIN              GPIO_PIN_1


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
    HWREG(LCD_RS_BASE + 0x00000000 + (LCD_RS_PIN << 2)) = ucByte;		\
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
extern void lcd_init(void);
extern void lcd_backlight_on(void);
extern void lcd_backlight_off(void);
extern void lcd_ili932x_close(void *pvDisplayData);
extern void lcd_ili932x_flush(void *pvDisplayData);
extern unsigned short lcd_get_control(void);


void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned short Color);
void lcd_write_data(unsigned short usData);
void lcd_write_command(unsigned char ucData);
//void LCD_ShowString(unsigned int x,unsigned int y, unsigned char *p);
void LCD_ShowString(unsigned int x0,unsigned int y0,const unsigned char *pcStr);
//extern const tDisplay g_slcd_320x240_ili932x;
//void GLCD_Test(void);
//===================================================================================
// The dimensions of the LCD panel.
//===================================================================================

#endif // __S95196X240X320X16_ILI9320_H__
