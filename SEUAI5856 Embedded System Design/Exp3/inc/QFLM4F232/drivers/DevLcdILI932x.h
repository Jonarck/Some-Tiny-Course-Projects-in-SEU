/****************************************************************************************
** File Name  ��DevLcdILI932x.h                                                        **
** Description:																	                                       **
**                                                                                     **
**                                                                                     **
**                                                                                     **
=========================================================================================
** Author  :                                                                           **
** Version :                                                                           **
** History :                                                                           **
****************************************************************************************/
#ifndef __DEV_LCD_ILI932X_H__
#define __DEV_LCD_ILI932X_H__
//===================================================================================
// The dimensions of the LCD panel.
//===================================================================================
#define LCD_VERTICAL_MAX      240
#define LCD_HORIZONTAL_MAX    320

//������ɫ,������ɫ
extern unsigned long POINT_COLOR,BACK_COLOR; 

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

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



#define g_sLcdDisplay	 g_slcd_320x240_ili932x
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
//===================================================================================
extern void lcd_ili932x_flush(void *pvDisplayData);
extern void dev_lcd_init_ili932x(void);
extern void dev_lcd_ili932x_clear(unsigned long ulcolor);
extern void dev_lcd_ili932x_draw_rectangle(unsigned long x1,unsigned long y1,
                                   unsigned long x2,unsigned long y2,
                                   unsigned long ulValue,unsigned char mode);
											extern const tDisplay g_slcd_320x240_ili932x;
#endif

