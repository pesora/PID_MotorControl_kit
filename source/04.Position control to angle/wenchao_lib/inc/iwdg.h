/**
   *************************************************** ****************************
   * @file iwdg.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.8.21
   * @note This program is the underlying programming of IWDG in ARM (attached to the official underlying library package)
   *************************************************** ****************************
   */

#ifndef __CH_LIB_IWDG_H__
#define __CH_LIB_IWDG_H__

#ifdef __cplusplus
  extern "C" {
#endif


#include "stm32f10x_iwdg.h"
//13104 0
/* API functions */
void IWDG_QuickInit(int timeInMs);
void IWDG_ENABLE(void);
void IWDG_FEED(void);

#endif
