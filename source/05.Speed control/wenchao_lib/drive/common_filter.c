/**
   *************************************************** ****************************
   * @file common_filter.c
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.10.16
   * @note This program is for application programming in ARM
   *************************************************** ****************************
   */
#include "sysinc.h"


  /**
  * @brief Initialization of first-order filter parameters, each defined structure must be initialized
  * @code
  * //Initialize structure First_order_x
  * First_Order_parameter_init(First_complementary_x, 0.15);
  * @endcode
  * @param[in] First_Order_parameter *p First_Order parameter structure
  * @param[in] ki ratio value
  */
void First_Order_parameter_init(FIRST_ORDER_PARAMETER *p,float ki)
{
	p->k = ki;
	p->num = 1;
}


/**
  * @brief first-order filter
  * @code
  * //Structure First_Order_x
  * First_Order_filter(First_Order_x, 45);
  * @endcode
  * @param[in] First_Order_parameter *p,First_Order parameter structure
  * @param[in] number updated data
  */
void First_Order_filter(FIRST_ORDER_PARAMETER *p, float number)
{
      p->num = p->k * number + (1-p->k) * p->num;
}