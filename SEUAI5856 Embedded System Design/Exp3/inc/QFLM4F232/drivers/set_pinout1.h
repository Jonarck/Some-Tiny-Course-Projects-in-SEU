/************************************************************************************
** File  Name  : Touch.c                                                           **
** Description : Functions related to pinout configuration.                        **
**               This file was modified from a sample available from the FatFs Web **
**               site. It was modified to work with a Stellaris QF-LM4F232H5QC     **
**               evaluation board.                                                 **
**               This is part of revision 8034 of the EK-LM4F232 Firmware Package. **
** Notes       : Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros     **
**               are platform dependent.                                           **
=====================================================================================
** Author Name : Triton.zhang                                                      **
** Data        : 2011-10-20                                                        **
** Notes       :                                                                   **
************************************************************************************/
#ifndef __SET_PINOUT_H__
#define __SET_PINOUT_H__

//===================================================================================
// An enum defining the various daughter boards that can be attached to an
// LM4F232H5QC  development board.
//===================================================================================
typedef enum
{
    DAUGHTER_NONE = 0,
    DAUGHTER_RF_MODUEL = 1,
    DAUGHTER_UNKNOWN = 0xFFFF
}tDaughterBoard;

//===================================================================================
// Macro allowing us to pack the fields of a structure.
//===================================================================================
#if defined(ccs) ||             \
    defined(codered) ||         \
    defined(gcc) ||             \
    defined(rvmdk) ||           \
    defined(__ARMCC_VERSION) || \
    defined(sourcerygxx)
#define PACKEDSTRUCT __attribute__ ((packed))
#elif defined(ewarm)
#define PACKEDSTRUCT
#else
#error Unrecognized COMPILER!
#endif


//===================================================================================
// Public function prototypes.
//===================================================================================
extern void PinoutSet(void);

#endif // __SET_PINOUT_H__
