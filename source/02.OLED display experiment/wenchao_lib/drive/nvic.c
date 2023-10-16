/**
   *************************************************** ****************************
   * @file nvic.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.21
   * @note This program is the underlying programming of NVIC in ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */
#include "nvic.h"


/**
  * @brief Set NVIC interrupt group 2: 2-bit preemption priority, 2-bit response priority

  */
void NVIC_init(void)
{

     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
