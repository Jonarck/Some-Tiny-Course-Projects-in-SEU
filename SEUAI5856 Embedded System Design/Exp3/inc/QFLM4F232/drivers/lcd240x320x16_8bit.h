//*****************************************************************************
//
// lcd240x320x16_8bit.h - Prototypes for the LCD controller.
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
// This is part of revision 5570 of the RDK-IDM Firmware Package.
//
//*****************************************************************************

#ifndef __LCD240X320X16_8BIT_H__
#define __LCD240X320X16_8BIT_H__

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Lcd240x320x16_8bitInit(void);
extern void Lcd240x320x16_8bitBacklightOn(void);
extern unsigned short Lcd240x320x16_8bitControllerIdGet(void);
extern void Lcd240x320x16_8bitBacklightOff(void);
extern const tDisplay g_sLcd240x320x16_8bit;
extern unsigned short g_usController;

#endif // __FORMIKE240X320X16_ILI9320_H__
