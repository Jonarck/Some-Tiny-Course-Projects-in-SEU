/****************************************************************************************
** File Name  £ºBoardHal.h                                                             **
** Description:																	                                       **
**                                                                                     **
**                                                                                     **
**                                                                                     **
=========================================================================================
** Author  :                                                                           **
** Version :                                                                           **
** History :                                                                           **
****************************************************************************************/
#ifndef __BOARD_HAL_H__
#define __BOARD_HAL_H__

//=========================================================================================
// Hardware XTAL define
//=========================================================================================
#define	HAL_BOARD_XTAL	SYSCTL_XTAL_16MHZ

//=========================================================================================
// LED1 to LED3 define
//=========================================================================================
#define HAL_LED1_PERIPH  		  SYSCTL_PERIPH_GPIOC
#define HAL_LED1_PORT		  	  GPIO_PORTC_BASE
#define	HAL_LED1_PIN			    GPIO_PIN_7

//=========================================================================================
// LCD Interface define
//=========================================================================================
#define HAL_LCD_CTL_PERIPH  		SYSCTL_PERIPH_GPIOE
#define HAL_LCD_CTL_PORT		  	GPIO_PORTE_BASE
#define	HAL_LCD_RS_PIN		      GPIO_PIN_2
#define	HAL_LCD_RD_PIN		      GPIO_PIN_0
#define	HAL_LCD_WR_PIN		      GPIO_PIN_1
#define	HAL_LCD_CS_PIN		      GPIO_PIN_3

#define HAL_LCD_BL_PERIPH  		  SYSCTL_PERIPH_GPIOJ
#define HAL_LCD_BL_PORT		  	  GPIO_PORTJ_BASE
#define	HAL_LCD_BL_PIN		      GPIO_PIN_2

#define HAL_LCD_DATAH_PERIPH  	SYSCTL_PERIPH_GPIOA
#define HAL_LCD_DATAH_PORT      GPIO_PORTA_BASE
#define	HAL_LCD_DATAH_PINS      0xFF

#define HAL_LCD_DATAL_PERIPH  	SYSCTL_PERIPH_GPIOD
#define HAL_LCD_DATAL_PORT      GPIO_PORTD_BASE
#define	HAL_LCD_DATAL_PINS      0xFF

#define HAL_LCD_RST_PERIPH  		SYSCTL_PERIPH_GPIOF
#define HAL_LCD_RST_PORT		  	GPIO_PORTF_BASE
#define	HAL_LCD_RST_PIN		      GPIO_PIN_6


#endif


