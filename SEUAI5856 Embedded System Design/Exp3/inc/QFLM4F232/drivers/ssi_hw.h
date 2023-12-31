//*****************************************************************************
//
// ssi_hw.h - Board connection information for the SDCard reader.
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
// This is part of revision 5961 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#ifndef __SSI_HW_H__
#define __SSI_HW_H__

//*****************************************************************************
//
// Peripheral definitions for the qf-lm4s232 board
//
//*****************************************************************************

//*****************************************************************************
//
// Serial flash SSI port
//
//*****************************************************************************
#define SFLASH_SSI_PERIPH       SYSCTL_PERIPH_SSI3
#define SFLASH_SSI_BASE         SSI3_BASE

//*****************************************************************************
//
// GPIO for serial flash pins
//
//*****************************************************************************
#define SFLASH_SSI_GPIO_PERIPH SYSCTL_PERIPH_GPIOH
#define SFLASH_SSI_GPIO_BASE   GPIO_PORTH_BASE
#define SFLASH_SSI_TX          GPIO_PIN_3
#define SFLASH_SSI_RX          GPIO_PIN_2
#define SFLASH_SSI_CLK         GPIO_PIN_0
#define SFLASH_SSI_PINS        (SFLASH_SSI_TX | SFLASH_SSI_RX | SFLASH_SSI_CLK)

//*****************************************************************************
//
// GPIO for the serial flash chip select.  This device shares the SSI bus.
//
//*****************************************************************************
#define SFLASH_CS_PERIPH   SYSCTL_PERIPH_GPIOH
#define SFLASH_CS_BASE     GPIO_PORTH_BASE
#define SFLASH_CS_PIN      GPIO_PIN_1

//*****************************************************************************
//
// SDCard SSI port
//
//*****************************************************************************
#define SDC_SSI_BASE            SSI3_BASE
#define SDC_SSI_SYSCTL_PERIPH   SYSCTL_PERIPH_SSI3

//*****************************************************************************
//
// GPIO for SDCard SSI pins
//
//*****************************************************************************
#define SDC_GPIO_PORT_BASE      GPIO_PORTK_BASE
#define SDC_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOK
#define SDC_SSI_CLK             GPIO_PIN_0
#define SDC_SSI_TX              GPIO_PIN_3
#define SDC_SSI_RX              GPIO_PIN_2

#define SDC_SSI_PINS            (SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK)

//*****************************************************************************
//
// GPIO for the SDCard chip select
//
//*****************************************************************************
#define SDCARD_CS_PERIPH   SYSCTL_PERIPH_GPIOK
#define SDCARD_CS_BASE     GPIO_PORTK_BASE
#define SDCARD_CS_PIN      GPIO_PIN_1



#endif // __SSI_HW_H__
