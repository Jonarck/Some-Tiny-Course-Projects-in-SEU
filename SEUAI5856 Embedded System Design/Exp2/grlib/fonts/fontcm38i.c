//*****************************************************************************
//
// fontcm38i.c - Font definition for the 38pt Cm italic font.
//
// Copyright (c) 2011 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 8034 of the Stellaris Graphics Library.
//
//*****************************************************************************

//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include "grlib/grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Characters: 32 to 126 inclusive
//     Style: cm
//     Size: 38 point
//     Bold: no
//     Italic: yes
//     Memory usage: 4896 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 38 point Cm italic font.
// Contains characters 32 to 126 inclusive.
//
//*****************************************************************************
static const unsigned char g_pucCm38iData[4693] =
{
      5,  15,   0,  69,  48,  32,  13, 240,  82, 164, 147, 163,
    163, 148, 147, 163, 163, 162, 178, 163, 162, 178, 178, 177,
    178, 178, 177, 193,   0,   6,  19, 148, 148, 147,   0,  15,
    112,  27,  15,   0,   5, 115,  67,  68,  52,  68,  52,  68,
     52, 113,  97,  97,  97,  97,  97, 113,  97,  97,  97,  97,
     82,   0,  45, 112,  98,  29,   0,  12, 114,  97, 240,  81,
    113, 240,  66,  98, 240,  65, 113, 240,  66,  98, 240,  65,
     98, 240,  66,  98, 240,  65,  98, 240,  66,  97, 240,  81,
     98, 240,  66,  97, 240,  81,  98, 191,   9, 209,  98, 240,
     66,  97, 240,  81,  98, 240,  66,  97, 240,  81,  98, 240,
     66,  97, 223,   9, 178,  97, 240,  81,  98, 240,  66,  97,
    240,  81,  98, 240,  66,  97, 240,  81,  98, 240,  66,  97,
    240,  81,  98, 240,  66,  97, 240,  81,  98, 240,  66,  97,
    240,  81,  98,   0,   9,  80,  62,  26,   0,  11, 101, 240,
     82,  65, 240,  35,  97, 243, 113, 242, 114, 227,  99, 226,
    114, 242, 240, 131, 240, 130, 240, 146, 240, 146, 240,  90,
    240,  66, 240, 146, 240, 131, 240, 130, 240, 146, 240, 131,
    240, 131, 240,  55, 161, 113,  67, 161,  97,  85, 113, 113,
     65,  51,  66, 133, 101,   0,  30,  64,  95,  28, 116, 209,
    146,  33, 194, 130,  65, 162, 130,  83,  84, 146,  81,  37,
     18, 146,  97, 130, 146,  97, 114, 147,  97,  98, 162,  97,
     98, 178,  97,  82, 194,  97,  66, 210,  81,  66, 226,  81,
     66, 241,  65,  66, 240,  18,  33,  66, 240,  66,  66, 240,
    162,  85, 242,  82,  65, 210,  82,  81, 210,  82,  81, 194,
     82,  97, 178,  98,  97, 162,  99,  97, 146, 114,  97, 146,
    130,  97, 130, 146,  82, 130, 146,  81, 130, 162,  65, 130,
    194,  34, 129, 227,   0,  25,  96,  86,  28, 240, 240, 115,
    240, 130,  49, 240,  98,  81, 131, 130,  97, 130, 131,  97,
    145, 130,  98, 145, 115,  98, 129, 130, 113, 129, 146, 241,
    162, 225, 178,  35, 115, 180,  49,  82,  18, 163,  65,  65,
     65, 162,  81,  49,  97, 132,  49,  49, 113, 115,  20,  65,
    113, 114, 164,  65, 114, 163,  81,  98, 240,  65,  98, 240,
     49, 114, 240,  49, 114, 240,  33, 130, 240,  18, 146, 226,
    178, 193, 227, 115, 240,  40,   0,  33,  48,  16,   8, 240,
    179,  68,  68,  68, 113,  97,  97, 113,  97,  97,   0,  24,
    112,  46,  17, 209, 226, 226, 226, 226, 226, 241, 242, 226,
    242, 226, 242, 226, 242, 226, 242, 242, 227, 226, 242, 242,
    227, 227, 226, 242, 242, 242, 242, 242, 242, 242, 240,  17,
    240,  18, 240,  17, 240,  17, 240,  33, 240,  17, 208,  40,
     16, 145, 242, 241, 242, 241, 241, 242, 226, 226, 226, 226,
    226, 226, 226, 226, 211, 210, 226, 226, 226, 210, 226, 226,
    210, 226, 226, 210, 225, 226, 210, 226, 210, 210, 225, 225,
    225, 225, 240,  34,  17, 130, 241, 240,  17, 242, 162,  49,
     66,  83,  33,  35, 130,  17,  18, 195, 211, 194,  17,  18,
    131,  33,  35,  82,  65,  50, 162, 241, 240,  17, 242,   0,
     46,  51,  25,   0,  23,  65, 240, 145, 240, 130, 240, 130,
    240, 129, 240, 145, 240, 145, 240, 130, 240, 129, 240, 145,
    240, 145, 223,   7, 209, 240, 145, 240, 145, 240, 130, 240,
    129, 240, 145, 240, 145, 240, 130, 240, 130, 240, 129, 240,
    145,   0,  23, 112,  17,   9,   0,  27,  51,  84,  84,  84,
    129, 113, 129, 113, 113, 129,  98, 240, 160,   9,  12,   0,
     28,  73,  57,   0,  24,  48,  11,   7,   0,  21,  19,  52,
     52,  51,   0,   8,  48,  78,  25, 240,  97, 240, 130, 240,
    114, 240, 129, 240, 130, 240, 129, 240, 130, 240, 129, 240,
    130, 240, 114, 240, 129, 240, 130, 240, 129, 240, 130, 240,
    129, 240, 130, 240, 114, 240, 129, 240, 130, 240, 129, 240,
    130, 240, 114, 240, 129, 240, 130, 240, 129, 240, 130, 240,
    129, 240, 130, 240, 114, 240, 129, 240, 130, 240, 129, 240,
    130, 240, 129, 240, 130, 240, 114, 240, 129, 240, 144,  55,
     19,   0,   8,  37, 194,  66, 162,  97, 146, 114, 114, 130,
    114, 130,  98, 146,  98, 146,  83, 131,  82, 147,  82, 146,
     83, 146,  83, 131,  82, 147,  82, 146,  83, 146,  83, 131,
     82, 146,  98, 146,  98, 130, 114, 130, 114, 114, 145,  98,
    162,  66, 212,   0,  22, 112,  34,  16,   0,   7,  65, 226,
    210, 211, 178,  18, 147,  19, 210, 226, 226, 211, 210, 226,
    226, 211, 210, 226, 226, 211, 210, 226, 226, 211, 210, 226,
    155,   0,  18,  80,  64,  20,   0,   8,  85, 225,  66, 193,
     98, 161, 129, 145, 146, 129,  33,  98, 113,  49,  98, 113,
     49,  98,  97,  65,  98,  97,  49,  98, 113,  49,  98, 113,
     33,  98, 146,  98, 240,  34, 240,  34, 240,  18, 240,  18,
    240,  34, 240,  33, 240,  49, 177,  97, 177,  98, 162, 102,
     82,  98,  56, 113, 101,   0,  23,  64,  58,  20,   0,   8,
     85, 210,  66, 162, 129, 145, 146, 113, 162,  98,  49,  98,
     97,  65,  98,  97,  65,  98,  97,  49,  98, 131, 113, 240,
     49, 240,  34, 228, 240,  66, 240,  65, 240,  66, 240,  50,
    240,  50, 129, 146, 114, 131, 114, 130, 129, 130, 145, 114,
    177,  67, 213,   0,  24,  16,  60,  18,   0,   8,  50, 240,
     18, 240,  18, 243, 242, 240,  18, 243, 242, 240,  18, 242,
    240,  18, 243, 242, 242, 240,  18, 242, 240,  18,  49, 178,
     50, 162,  66, 146,  67, 131,  66, 131,  82, 115,  98, 104,
     18, 113, 119, 194, 240,  18, 242, 240,  18, 240,  18, 240,
     18, 240,  17, 240, 240, 240,  53,  20,   0,   8,  34, 114,
    154, 168, 198, 209, 240,  65, 240,  65, 240,  65, 240,  49,
    240,  65,  37, 195,  66, 177, 113, 161, 130, 240,  50, 240,
     50, 240,  50, 240,  50, 114, 131,  99, 130, 114, 131, 113,
    146, 129, 130, 146,  98, 178,  66, 213,   0,  24,  16,  57,
     19,   0,   8,  68, 210,  65, 162,  97, 146,  98, 130, 114,
    115, 114, 114, 240,  19, 240,  18, 240,  19,  37, 146,  33,
     66, 115,  17,  98, 100, 114,  99, 130,  99, 130,  83, 146,
     83, 131,  82, 147,  82, 147,  82, 131,  98, 130, 114, 115,
    129,  99, 146,  66, 212,   0,  22, 112,  62,  20,   0,   8,
      1,  20,  97,  97,  21,  81, 115,  49,  81,  99,  81,  49,
    114,  97,  33, 129, 116, 113, 161, 129, 161, 113, 161, 129,
    161, 240,  50, 240,  34, 240,  50, 240,  34, 240,  50, 240,
     35, 240,  34, 240,  50, 240,  35, 240,  34, 240,  35, 240,
     35, 240,  34, 240,  35, 240,  35,   0,  24,  16,  51,  20,
      0,   8,  85, 210,  67, 162, 114, 130, 130, 129, 146, 114,
    146, 114, 145, 131, 114, 131,  98, 149,  50, 183, 229, 246,
    194,  52, 162,  84, 130, 115, 114, 146, 114, 146,  98, 162,
     98, 161, 114, 146, 114, 145, 146, 113, 163,  66, 213,   0,
     24,  56,  19,   0,   8,  37, 194,  66, 162,  97, 146, 114,
    114, 130, 114, 130,  98, 146,  98, 146,  83, 131,  83, 131,
     82, 147,  82, 131,  98, 131,  98, 116, 113,  97,  18, 130,
     65,  34, 164,  35, 240,  18, 240,  19, 240,  18, 114, 114,
    130,  98, 145,  98, 162,  66, 197,   0,  23,  17,  10,   0,
     15,  67, 100, 100, 114,   0,  10,  82, 116, 100,  99,   0,
     12,  16,  24,  12,   0,  18,  99, 132, 132, 131,   0,  12,
    114, 148, 132, 131, 177, 177, 161, 177, 161, 161, 161, 240,
    240,  80,  32,  13,   0,  15,  67, 148, 148, 147,   0,   6,
     17, 193, 178, 178, 177, 178, 178, 178, 163, 162, 178, 163,
    163, 163, 148, 147, 163, 163, 148, 162, 240, 128,  13,  27,
      0,  50, 127,   7,   0,  20,  95,   7,   0,  47, 112,  45,
     16,   0,  19,  19, 211, 211, 210,   0,   7,  67, 194,  33,
    162,  49, 161,  65, 226, 210, 195, 195, 195, 194,  98,  82,
     99,  66, 115,  65, 145,  66, 145,  65, 145,  81, 145,  81,
    129,  98,  97, 130,  50, 165, 240, 160,  45,  16, 240, 132,
    162,  65, 129, 113,  97, 129,  81, 145,  81, 145,  65, 161,
     65, 145,  67, 114,  67,  98,  82,  98, 195, 195, 195, 194,
    210, 225,  65, 161,  50, 161,  34, 195,   0,   7,  66, 211,
    211, 211,   0,  19,  64,  87,  26,   0,  11,  22, 240,  50,
     99, 210, 162, 177, 210, 145,  84,  97, 129,  66,  50,  82,
     97,  51,  82,  81,  82,  50, 115,  49,  81,  50, 131,  49,
     65,  50, 146,  65,  65,  50, 146,  65,  65,  35, 131,  65,
     49,  50, 146,  65,  65,  50, 146,  65,  65,  50, 131,  65,
     65,  50, 131,  49,  81,  50, 115,  65,  81,  66,  84,  49,
     97,  67,  50,  18,  34, 113,  84,  67, 145, 240, 162, 240,
    162, 195, 162, 131, 248,   0,  30, 112,  68,  26, 240, 240,
    210, 240, 146, 240, 131, 240, 131, 240, 116, 240, 116, 240,
     97,  19, 240,  81,  35, 240,  81,  35, 240,  65,  66, 240,
     65,  66, 240,  49,  82, 240,  49,  83, 240,  17,  99, 240,
     17,  99, 241, 115, 225, 131, 236, 209, 147, 209, 147, 193,
    163, 193, 163, 177, 179, 162, 179, 161, 210, 146, 196,  87,
    136,   0,  29,  80,  56,  27,   0,  10, 127, 243, 131, 210,
    163, 194, 163, 179, 163, 179, 163, 178, 179, 178, 163, 179,
    162, 195, 146, 210, 131, 234, 240,  19, 131, 211, 147, 194,
    179, 178, 179, 163, 179, 163, 179, 162, 195, 162, 179, 163,
    179, 163, 163, 178, 163, 179, 132, 159,   1,   0,  31,  96,
     62,  26,   0,  11,  22,  81, 194,  97,  50, 162, 145,  18,
    162, 179, 146, 195, 130, 210, 130, 241, 129, 240,  17, 114,
    240,  17, 113, 240,  17, 114, 240, 146, 240, 131, 240, 130,
    240, 146, 240, 146, 240, 146, 241, 130, 241, 130, 241, 145,
    225, 162, 193, 178, 177, 210, 145, 243,  82, 240,  54,   0,
     31,  16,  56,  28,   0,  11,  46, 240,  35, 130, 243, 146,
    226, 178, 195, 178, 195, 194, 179, 194, 178, 210, 163, 210,
    163, 210, 163, 210, 162, 226, 147, 211, 147, 210, 163, 210,
    162, 211, 147, 210, 163, 210, 163, 194, 178, 194, 179, 193,
    195, 177, 211, 146, 211, 131, 190,   0,  33,  32,  65,  28,
      0,  11,  63,   3, 195, 163, 195, 178, 195, 178, 195, 178,
    194, 193, 195, 193, 195,  97,  81, 194, 113, 240,  50, 113,
    240,  35,  98, 240,  42, 240,  50,  98, 240,  50, 113, 240,
     35, 113, 240,  35,  97,  97, 178, 225, 178, 209, 179, 209,
    179, 194, 178, 209, 194, 194, 179, 178, 195, 148, 143,   5,
      0,  32,  64,  64,  28,   0,  11,  63,   3, 195, 148, 195,
    177, 211, 177, 211, 177, 210, 193, 195, 193, 195, 193, 195,
     97, 240,  50, 113, 240,  35,  98, 240,  35,  82, 240,  58,
    240,  50,  98, 240,  35,  98, 240,  35,  97, 240,  51,  97,
    240,  50, 240, 163, 240, 163, 240, 163, 240, 162, 240, 163,
    240, 163, 240, 107,   0,  33,  80,  61,  27,   0,  11,  86,
     81, 211,  82,  34, 179, 133, 162, 179, 147, 210, 131, 226,
    130, 242, 115, 241, 115, 240,  17, 115, 240,  17,  99, 240,
    147, 240, 147, 240, 131, 240, 147, 240, 147, 169,  83, 227,
    115, 227, 115, 226, 146, 211, 147, 195, 162, 180, 163, 148,
    211,  82,  49, 246,  81,   0,  31,  64,  61,  31,   0,  12,
     57,  73, 195, 163, 242, 163, 243, 163, 243, 163, 243, 162,
    240,  18, 163, 243, 163, 243, 163, 243, 162, 240,  18, 163,
    255,   1, 243, 163, 243, 162, 240,  18, 163, 243, 163, 243,
    163, 243, 162, 240,  18, 163, 243, 163, 243, 163, 243, 162,
    240,  18, 178, 243, 163, 201,  73,   0,  36,  54,  19,   0,
      7, 122, 195, 240,  19, 240,  19, 240,  18, 240,  19, 240,
     19, 240,  19, 240,  18, 240,  19, 240,  19, 240,  19, 240,
     18, 240,  19, 240,  19, 240,  19, 240,  18, 240,  19, 240,
     19, 240,  19, 240,  18, 240,  19, 240,  19, 240,  19, 202,
      0,  22,  64,  55,  21,   0,   9,   9, 240,  19, 240,  50,
    240,  66, 240,  51, 240,  51, 240,  50, 240,  51, 240,  51,
    240,  51, 240,  50, 240,  51, 240,  51, 240,  51, 240,  50,
    240,  51, 240,  51, 240,  51, 240,  50, 178,  83, 163,  83,
    162,  83, 177,  83, 209,  51, 244,   0,  25,  64,  77,  32,
      0,  12, 121, 103, 196, 179, 227, 178, 240,  19, 162, 240,
     35, 146, 240,  50, 145, 240,  67, 129, 240,  83,  98, 240,
     99,  82, 240, 114,  82, 240, 115,  67, 240, 115,  52, 240,
    115,  18,  19, 240, 114,  18,  51, 240,  85,  67, 240,  83,
     99, 240,  83, 115, 240,  66, 131, 240,  51, 147, 240,  35,
    147, 240,  35, 147, 240,  34, 179, 243, 179, 243, 180, 170,
    104,   0,  37,  57,  24,   0,   9, 122, 240,  20, 240,  83,
    240,  99, 240,  99, 240,  98, 240,  99, 240,  99, 240,  99,
    240,  98, 240,  99, 240,  99, 240,  99, 240,  98, 240,  99,
    240,  99, 240,  99, 162, 146, 177, 147, 177, 147, 162, 147,
    161, 162, 162, 147, 146, 163, 116, 111,   3,   0,  27,  96,
     99,  36,   0,  14,  38, 246, 195, 228, 225,  18, 209,  19,
    225,  18, 209,  18, 241,  19, 177,  19, 241,  19, 177,  19,
    225,  35, 161,  35, 225,  35, 145,  50, 241,  50, 145,  35,
    241,  50, 129,  51, 226,  50, 129,  51, 225,  66, 113,  66,
    241,  66,  97,  67, 241,  66,  97,  67, 226,  67,  65,  83,
    225,  83,  65,  82, 241,  83,  49,  83, 241,  83,  33,  99,
    226,  83,  33,  98, 241, 114,  17, 114, 241, 114,  17, 114,
    241, 115, 115, 226, 115, 114, 227, 114, 130, 199,  81,  89,
      0,  41,  80,  95,  32,   0,  12, 118, 136, 211, 163, 241,
     18, 177, 240,  17,  19, 161, 240,  17,  19, 146, 240,  17,
     35, 129, 240,  17,  51, 129, 240,  17,  51, 129, 240,  17,
     67,  98, 240,  17,  67,  97, 240,  17,  83,  97, 240,  17,
     99,  81, 240,  17,  99,  66, 240,  17,  99,  65, 240,  17,
    131,  49, 240,  17, 131,  49, 240,  17, 146,  34, 240,  17,
    147,  17, 240,  17, 163,  17, 240,  17, 178,  17, 240,  17,
    180, 240,  17, 179, 240,  17, 210, 243, 194, 200, 162,   0,
     37,  64,  65,  26,   0,  11,  38, 240,  51,  82, 226, 146,
    194, 178, 147, 194, 146, 226, 114, 242,  99, 242,  83, 240,
     18,  83, 240,  18,  67, 240,  34,  67, 240,  34,  67, 240,
     19,  51, 240,  34,  67, 240,  34,  67, 240,  18,  83, 240,
     18,  83, 242,  99, 227, 114, 226, 131, 194, 162, 178, 179,
    131, 211,  83, 240,  39,   0,  31,  55,  27,   0,  10, 127,
    243, 115, 226, 162, 195, 163, 179, 163, 179, 163, 178, 179,
    163, 164, 163, 163, 179, 147, 194, 147, 195, 131, 219, 240,
     19, 240, 146, 240, 147, 240, 147, 240, 147, 240, 146, 240,
    147, 240, 147, 240, 147, 240, 146, 240, 147, 240, 105,   0,
     32,  80,  92,  25,   0,  10, 102, 240,  35,  82, 211, 130,
    178, 178, 146, 194, 130, 226,  99, 226,  98, 242,  82, 240,
     18,  82, 240,  18,  66, 240,  34,  66, 240,  34,  66, 240,
     19,  51, 240,  18,  66, 240,  34,  66, 240,  18,  82, 240,
     18,  82, 242,  98, 242, 113,  99,  82, 130,  65,  49,  50,
    161,  49,  65,  34, 178,  33,  65,  18, 210,  17,  66, 240,
     39, 240, 145,  97, 240,  33,  97, 240,  33,  81, 240,  50,
     50, 240,  54, 240,  70, 240,  83,   0,   7,  64,  59,  26,
      0,  10,  77, 243, 115, 211, 131, 195, 147, 179, 147, 178,
    163, 163, 163, 163, 163, 162, 163, 178, 147, 179, 131, 195,
    114, 234, 240,  18, 114, 227, 130, 211, 130, 210, 146, 210,
    146, 195, 146, 195, 131, 194, 147, 194, 147,  65,  99, 147,
     65,  99, 147,  49,  73, 117,   0,  29, 112,  58,  24,   0,
     10,  53,  65, 194,  81,  33, 193, 131, 177, 147, 162, 161,
    177, 177, 162, 177, 162, 177, 162, 240, 115, 240, 102, 240,
     72, 240,  56, 240,  69, 240, 115, 240, 114, 240, 114, 145,
    194, 145, 194, 145, 193, 146, 178, 147, 146, 163, 130, 177,
     34,  82, 177,  70,   0,  28,  80,  63,  25,   0,   9,  95,
      5,  82, 114, 114,  66, 115, 114,  65, 131, 129,  65, 131,
    129,  49, 146, 145,  49, 131, 129,  65, 131, 129, 211, 240,
    114, 240, 115, 240, 115, 240, 115, 240, 114, 240, 115, 240,
    115, 240, 115, 240, 114, 240, 115, 240, 115, 240, 115, 240,
    114, 240, 115, 240, 115, 240,  44,   0,  29,  96,  58,  26,
      0,   9, 121,  88, 100, 178, 147, 193, 163, 193, 163, 193,
    162, 193, 163, 193, 163, 193, 163, 193, 162, 193, 178, 193,
    163, 193, 162, 209, 162, 193, 178, 193, 163, 193, 162, 193,
    178, 193, 178, 193, 178, 177, 194, 161, 225, 146, 226, 114,
    240,  18,  66, 240,  69,   0,  31,  64,  65,  27,   0,  10,
     24, 151,  84, 210, 131, 225, 162, 209, 179, 193, 179, 177,
    195, 161, 211, 161, 211, 145, 227, 130, 227, 129, 243, 113,
    240,  19, 113, 240,  34,  97, 240,  51,  65, 240,  67,  65,
    240,  67,  49, 240,  83,  34, 240,  83,  33, 240,  99,  17,
    240, 115,  17, 240, 116, 240, 146, 240, 162, 240, 161,   0,
     33, 100,  36,   0,  13,  72,  73,  87,  83, 163, 163, 115,
    163, 162, 131, 163, 161, 147, 163, 145, 163, 148, 145, 163,
    148, 129, 179, 129,  19, 129, 179, 114,  19, 113, 195, 113,
     35,  98, 195,  97,  51,  97, 226,  97,  51,  81, 242,  81,
     67,  81, 242,  81,  67,  65, 240,  18,  65,  83,  65, 240,
     19,  34,  83,  49, 240,  35,  33,  99,  49, 240,  35,  17,
    115,  33, 240,  51,  17, 115,  18, 240,  52, 131,  17, 240,
     68, 147, 240,  83, 163, 240,  83, 162, 240,  98, 178, 240,
     97, 193,   0,  42, 112,  74,  31,   0,  12,  57, 103, 196,
    147, 240,  19, 145, 240,  51, 129, 240,  68,  98, 240,  83,
     97, 240,  99,  81, 240, 131,  49, 240, 147,  33, 240, 163,
     17, 240, 195, 240, 211, 240, 211, 240, 212, 240, 177,  19,
    240, 161,  36, 240, 129,  67, 240, 113,  83, 240,  97, 115,
    240,  65, 131, 240,  50, 131, 240,  49, 163, 240,  17, 179,
    227, 180, 167, 136,   0,  35, 112,  62,  27,   0,  10,  24,
    151,  84, 210, 147, 194, 163, 193, 179, 177, 211, 145, 227,
    129, 243, 114, 240,  19,  97, 240,  35,  81, 240,  51,  65,
    240,  83,  33, 240,  99,  17, 240, 117, 240, 131, 240, 146,
    240, 147, 240, 147, 240, 147, 240, 146, 240, 147, 240, 147,
    240, 147, 240, 131, 240, 105,   0,  32,  64,  58,  26,   0,
     10,  95,   1, 164, 131, 163, 147, 178, 147, 193, 163, 178,
    147, 193, 147, 209, 131, 240, 115, 240, 115, 240, 131, 240,
    115, 240, 115, 240, 115, 240, 115, 240, 115, 129, 211, 145,
    211, 130, 195, 145, 195, 161, 179, 162, 163, 162, 163, 163,
    163, 133, 159,   1,   0,  30,  64,  40,  16, 148, 178, 226,
    225, 241, 226, 226, 225, 241, 226, 226, 225, 241, 226, 226,
    225, 241, 226, 226, 225, 241, 226, 226, 225, 241, 226, 226,
    225, 241, 226, 226, 225, 241, 226, 226, 225, 244, 192,  27,
     15,   0,   6,  18,  81,  97,  97,  97,  97,  97,  97, 113,
     97,  97,  97, 116,  52,  68,  52,  68,  52,  82,  82,   0,
     45,  80,  50,  17, 149, 242, 242, 241, 240,  17, 242, 242,
    241, 240,  17, 242, 242, 241, 240,  17, 242, 242, 241, 240,
     17, 242, 242, 241, 240,  17, 242, 242, 241, 240,  17, 242,
    242, 241, 240,  17, 240,  17, 242, 241, 240,  17, 240,  17,
    242, 241, 197, 192,  15,  12, 240, 240, 177, 163, 115,  18,
     82,  65,  65, 113,   0,  43, 112,  10,   7, 240, 115,  52,
     52,  51,   0,  26,  96,  16,   8, 240, 209,  97,  97,  97,
    113,  97, 116,  68,  68,  82,   0,  24,  80,  44,  18,   0,
     27, 100, 194,  52, 130,  83, 115,  83, 114,  99,  99,  98,
    114, 114,  99, 114,  99,  99,  98, 114, 114, 114,  49,  50,
    114,  49,  50,  99,  34,  65,  83,  49,  82,  49,  34,  18,
    100,  67,   0,  20, 112,  47,  15,   0,   5, 117, 195, 194,
    210, 210, 195, 194, 210, 210, 195,  20, 114,  17,  50,  99,
     81,  98,  98,  67,  98,  66, 114,  66, 114,  51,  99,  51,
     99,  50, 114,  66, 114,  66,  98,  82,  98,  97,  82, 114,
     50, 148,   0,  18,  31,  15,   0,  23,  52, 146,  65, 114,
     66,  98,  82,  82,  98,  67, 194, 195, 194, 210, 210, 210,
    145,  50, 130,  66,  98,  82,  66, 148,   0,  17, 112,  62,
     19,   0,   8,  69, 240,  19, 240,  18, 240,  34, 240,  34,
    240,  19, 240,  18, 240,  34, 240,  34, 180,  19, 146,  52,
    146,  83, 131,  83, 130,  99, 115,  98, 130, 114, 115, 114,
    115,  99, 114, 114, 130, 114,  49,  66,  99,  49,  66,  99,
     34,  81,  83,  49,  98,  49,  34,  18, 116,  67,   0,  22,
     16,  31,  16,   0,  24, 117, 146,  66,  99,  97,  83, 113,
     82, 129,  67, 113,  82,  98,  89, 115, 210, 226, 226, 226,
    161,  65, 130,  82,  82, 149,   0,  19,  67,  20,   0,   9,
     19, 240,  18,  33, 226,  34, 211, 240,  34, 240,  50, 240,
     35, 240,  35, 240,  34, 234, 211, 240,  35, 240,  34, 240,
     50, 240,  50, 240,  35, 240,  35, 240,  34, 240,  50, 240,
     35, 240,  35, 240,  35, 240,  34, 240,  50, 240,  35, 240,
     35, 240,  34, 240,  50, 210,  34, 226,  34, 225,  34, 240,
     19,   0,   7,  50,  18,   0,  28,   4, 194,  52, 130,  83,
    114,  99, 114,  99,  98, 114, 114, 114,  99, 114,  99,  99,
     98, 114, 114, 114, 114,  99, 114,  99, 129,  83, 146,  49,
     18, 164,  19, 243, 242, 240,  18, 114,  99, 115,  67, 130,
     67, 166,   0,   5, 112,  62,  19,   0,   7,  69, 240,  19,
    240,  18, 240,  34, 240,  34, 240,  19, 240,  18, 240,  34,
    240,  34, 240,  19,  36, 162,  18,  50, 148,  82, 131,  98,
    115, 114, 115, 114, 114, 115, 114, 114, 115, 114, 114, 114,
    130, 114,  49,  51,  99,  49,  51,  98,  65,  50, 114,  49,
     66, 114,  34,  66, 132,   0,  22,  38,  13,   0,   7,  50,
    178, 178,   0,   9,  19, 130,  34, 113,  50,  97,  66,  97,
     51,  81,  66, 163, 162, 178, 163, 162,  65,  83,  49,  98,
     65,  98,  49, 114,  34, 132,   0,  15,  48,  42,  17,   0,
     10,   2, 242, 242,   0,  12,   4, 193,  50, 161,  66, 145,
     82, 130,  82, 129,  83, 226, 242, 227, 227, 226, 242, 227,
    226, 242, 227, 227, 226, 227, 226, 146,  51, 146,  34, 196,
      0,   5,  96,  60,  18,   0,   7,  21, 240,  18, 240,  18,
    240,  18, 242, 240,  18, 240,  18, 240,  18, 242, 240,  18,
     98, 130,  66,  33,  99,  49,  50,  98,  49,  66,  98,  33,
    210,  17, 212, 226,  19, 194,  50, 178,  66, 147,  66, 146,
     82,  49,  82,  82,  49,  82,  82,  49,  67,  82,  33,  82,
    115,   0,  21,  37,  11, 240, 240,  86, 130, 146, 146, 131,
    130, 146, 131, 130, 146, 146, 131, 130, 146, 146, 131, 130,
    146, 146, 131,  49,  66,  49,  82,  49,  82,  49,  82,  33,
    115,   0,  13,  32,  67,  30,   0,  45,  35,  84,  84, 130,
     18,  34,  50,  34,  50, 113,  36,  85,  82,  81,  51, 100,
     98,  81,  51,  99, 114,  81,  50, 115, 114, 146, 114, 115,
    131, 114, 114, 146, 115, 114, 146, 114, 114, 147, 114, 114,
     49,  83,  99,  99,  49,  82, 115,  98,  65,  82, 114, 114,
     49,  83, 114, 114,  34,  82, 130, 132,   0,  34,  48,  50,
     21,   0,  31, 100,  68, 145,  33,  34,  50, 113,  52,  82,
     97,  51,  98,  81,  51, 114,  81,  51, 114, 146, 115, 146,
    114, 147, 114, 146, 114, 162, 114,  49,  98,  99,  49,  83,
     98,  65,  82, 114,  49,  98, 114,  34,  98, 132,   0,  24,
     32,  37,  17,   0,  26,  52, 178,  66, 130,  97, 114, 114,
     82, 130,  67, 130,  66, 146,  51, 146,  51, 131,  50, 146,
     66, 146,  66, 130,  82, 114, 114,  82, 130,  66, 180,   0,
     20,  48,  58,  21,   0,  32,  19,  68, 146,  18,  33,  50,
    129,  51,  81, 114,  35,  98,  97,  50, 114,  97,  50, 114,
    162, 114, 147,  99, 146, 115, 146, 114, 162, 114, 147,  98,
    163,  98, 163,  82, 180,  50, 179,  20, 210, 240,  66, 240,
     66, 240,  51, 240,  50, 240,  66, 240,  24,   0,   6, 112,
     48,  17,   0,  26,  36,  49, 114,  50,  17, 114,  83,  99,
     83,  98,  99,  83,  98,  98, 114,  83, 114,  83,  99,  82,
    114,  98, 114,  98,  99,  98,  99, 113,  83, 130,  49,  18,
    148,  34, 227, 226, 242, 242, 227, 226, 200, 240, 240, 160,
     33,  17,   0,  25, 100,  52,  82,  33,  33,  65,  65,  51,
     66,  65,  50,  82,  49,  51, 161,  50, 242, 242, 227, 226,
    242, 227, 226, 242, 242, 242,   0,  20,  96,  32,  16,   0,
     24, 101, 146,  81, 114, 113,  82,  99,  82,  99,  83, 215,
    168, 197, 211,  66, 130,  66, 130,  66, 129,  81, 129, 113,
     82, 149,   0,  19,  16,  35,  13,   0,   8, 114, 163, 163,
    162, 178, 163, 162, 122, 114, 163, 162, 178, 178, 163, 162,
    178, 178, 163,  65,  82,  81,  82,  66,  82,  65,  98,  49,
    132,   0,  15,  64,  50,  21,   0,  31, 116, 114, 114,  34,
     98, 113,  50,  98,  97,  66,  83,  97,  51,  83,  81,  66,
     98, 178,  98, 162,  99, 162,  98, 163,  98, 162, 114,  49,
     98,  99,  49,  98,  98,  49, 114,  83,  49, 130,  50,  17,
     34, 148,  67,   0,  24,  32,  43,  18,   0,  27,  52,  98,
     81,  50,  67,  81,  50,  67,  65,  66,  82,  65,  51,  82,
     49,  66,  98, 115,  97, 130, 113, 130, 113, 115,  97, 130,
    113, 130, 113, 130,  97, 146,  97, 162,  50, 196,   0,  21,
     48,  60,  25,   0,  37, 116, 113,  82,  81,  50,  82,  67,
     81,  50,  82,  67,  65,  66,  82,  82,  65,  51,  67,  82,
     49,  66,  82,  98, 115,  82,  97, 130,  98,  97, 130,  83,
     97, 115,  82,  98, 114,  98,  97, 130,  98,  97, 130,  98,
     81, 146,  83,  81, 162,  50,  18,  49, 197,  52,   0,  29,
     16,  48,  19,   0,  29,   4,  67, 114,  34,  33,  49,  81,
     83,  50,  81,  83,  50,  65,  98, 161,  83, 240,  19, 240,
     18, 240,  34, 240,  19, 240,  18,  97, 162,  97,  66,  51,
     81,  82,  51,  81,  81,  49,  34,  34, 115,  68,   0,  22,
     32,  58,  19,   0,  28, 116, 114,  82,  34,  98,  81,  50,
     98,  65,  66,  83,  65,  51,  83,  49,  66,  98, 146,  98,
    130,  99, 130,  98, 131,  98, 130, 114, 130,  99, 130,  98,
    146,  83, 162,  49,  18, 180,  19, 240,  18, 240,  34, 146,
     82, 147,  67, 146,  82, 161,  67, 197,   0,   6,  32,  32,
     18,   0,  27,  99,  81, 133,  49, 129,  69, 114,  98, 242,
    242, 242, 242, 242, 242, 242, 242, 114, 113, 129, 117,  50,
    114,  54,  98,  83,   0,  21,  32,   9,  20,   0,  45,  15,
      2,   0,  45,  48,  10,  36,   0,  81,  15,  15,   3,   0,
     81,  48,  19,  14,   0,   5,  97,  66,  98,  51,  82,  66,
     82,  66,  97,  66,  97,  81,   0,  49, 112,  16,  15,   0,
      6,   3,  81,  81,  33,  65,  81,  65,  33,  81,  83,   0,
     57,
};

//*****************************************************************************
//
// The font definition for the 38 point Cm italic font.
//
//*****************************************************************************
const tFont g_sFontCm38i =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    32,

    //
    // The height of the font.
    //
    37,

    //
    // The baseline of the font.
    //
    28,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   37,   64,  162,  224,  319,  405,
         421,  467,  507,  541,  592,  609,  618,  629,
         707,  762,  796,  860,  918,  978, 1031, 1088,
        1150, 1201, 1257, 1274, 1298, 1330, 1343, 1388,
        1433, 1520, 1588, 1644, 1706, 1762, 1827, 1891,
        1952, 2013, 2067, 2122, 2199, 2256, 2355, 2450,
        2515, 2570, 2662, 2721, 2779, 2842, 2900, 2965,
        3065, 3139, 3201, 3259, 3299, 3326, 3376, 3391,
        3401, 3417, 3461, 3508, 3539, 3601, 3632, 3699,
        3749, 3811, 3849, 3891, 3951, 3988, 4055, 4105,
        4142, 4200, 4248, 4281, 4313, 4348, 4398, 4441,
        4501, 4549, 4607, 4639, 4648, 4658, 4677,
    },

    //
    // A pointer to the actual font data
    //
    g_pucCm38iData
};
