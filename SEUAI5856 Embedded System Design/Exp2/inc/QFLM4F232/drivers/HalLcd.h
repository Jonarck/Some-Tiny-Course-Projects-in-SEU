/****************************************************************************************
** File Name  £ºHalLcd.h                                                               **
** Description:					                            										               **
**                                                                                     **
**                                                                                     **
**                                                                                     **
=========================================================================================
** Author  :                                                                           **
** Version :                                                                           **
** History :                                                                           **
****************************************************************************************/
#ifndef __HAL_LCD_H__
#define __HAL_LCD_H__


#define LCD_TYPE 0x9325

//=======================================================================================
//=======================================================================================
#define	 LCD_SET_RS(ucByte) \
{ \
    HWREG(HAL_LCD_CTL_PORT + GPIO_O_DATA + (HAL_LCD_RS_PIN << 2)) = ucByte;		\
}

#define	LCD_SET_RD(ucByte) \
{\
    HWREG(HAL_LCD_CTL_PORT + GPIO_O_DATA + (HAL_LCD_RD_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_WR(ucByte)	\
{								\
    HWREG(HAL_LCD_CTL_PORT + GPIO_O_DATA + (HAL_LCD_WR_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_CS(ucByte)	\
{								\
    HWREG(HAL_LCD_CTL_PORT + GPIO_O_DATA + (HAL_LCD_CS_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_BL(ucByte)	\
{								\
    HWREG(HAL_LCD_BL_PORT + GPIO_O_DATA + (HAL_LCD_BL_PIN << 2)) = ucByte;		\
}

#define	 LCD_SET_RST(ucByte)	\
{								\
    HWREG(HAL_LCD_RST_PORT + GPIO_O_DATA + (HAL_LCD_RST_PIN << 2)) = ucByte;		\
}



#define LCD_SET_DATAH(ucByte) \
{ \
		HWREG(HAL_LCD_DATAH_PORT + GPIO_O_DATA + (HAL_LCD_DATAH_PINS << 2)) = (ucByte);  \
}

#define LCD_SET_DATAL(ucByte) \
{ \
		HWREG(HAL_LCD_DATAL_PORT + GPIO_O_DATA + (HAL_LCD_DATAL_PINS << 2)) = ucByte;  \
}

//=======================================================================================
//=======================================================================================
extern void HAL_INIT_LCD(void);
extern void HAL_LCD_WRITE_DATA(unsigned short usData);
extern unsigned short HAL_LCD_READ_DATA(void);
extern void HAL_LCD_WRITE_COMMAND(unsigned char ucData);
extern void HAL_LCD_INIT_GPIO(void);

#endif

