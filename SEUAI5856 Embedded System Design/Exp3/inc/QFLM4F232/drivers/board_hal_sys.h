/*****************************************************************************
** File Name : board_hal_tick.h                                      		**
** 																			**
*****************************************************************************/

#ifndef _BOARD_HAL_TICK_H_
#define _BOARD_HAL_TICK_H_

//===================================================================================
// 定义系统tick时间参数，每秒和每毫秒的定时参数
//===================================================================================
#define SYSTICKS_PER_SECOND 100
#define SYSTICK_PERIOD_MS (1000 / SYSTICKS_PER_SECOND)



//====================================================================================
extern void hal_sys_delay(unsigned long ulSeconds);
extern void hal_clock_init(void);
#endif

