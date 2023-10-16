/**
   *************************************************** ****************************
   * @filepid.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.9.10
   * @note This program is PID application layer programming
   *************************************************** ****************************
   */

#include "sysinc.h"

  /**
  * @brief PID parameter initialization, every time a structure is defined, it must be initialized
  * @param[in] piddate: parameter structure
  */
void PID_DATE_Init(PID_DATE *piddate)
{
	piddate->pid_p = 50;
	piddate->pid_i = 0.1;
	piddate->pid_d = 6;
	piddate->pid_err = 0;
	piddate->pid_err_max = 3;
	piddate->pid_last_err = 0;
	piddate->pid_integral_err = 0;
	piddate->pid_integral_err_max = 300;
	piddate->pid_control = 0;
}

  /**
  * @brief PID control function
  * @param[in] target_val: the target value to be achieved
  * @param[in] now_val: current position value
  * @param[in] piddate: parameter structure
  */
void PID_Control(short target_val,short now_val,PID_DATE *piddate)
{
	piddate->pid_last_err = piddate->pid_err;
	piddate->pid_err = target_val - now_val;
	
	if(piddate->pid_err > piddate->pid_err_max)
	{
		piddate->pid_err = piddate->pid_err_max;
	}
	piddate->pid_integral_err += piddate->pid_err;
	
	if(piddate->pid_integral_err > piddate->pid_integral_err_max)
	{
		piddate->pid_integral_err = piddate->pid_integral_err_max;
	}
	piddate->pid_control = piddate->pid_p*piddate->pid_err + piddate->pid_i*piddate->pid_integral_err+
	piddate->pid_d*(piddate->pid_err-piddate->pid_last_err);
}

  /**
  * @brief Initialization of cascade PID parameters, each defined structure must be initialized
  * @param[in] pid_date: parameter structure
  */
void PID_CASCADE_DATE_Init(PID_CASCADE_DATE *pid_date)
{
	pid_date->pid_out_p = 30;
	pid_date->pid_out_i = 1;
	pid_date->pid_out_d = 6;
	pid_date->pid_out_err = 0;
	pid_date->pid_out_err_max = 3;
	pid_date->pid_out_last_err = 0;
	pid_date->pid_out_integral_err = 0;
	pid_date->pid_out_integral_err_max = 300;
	pid_date->pid_out_control = 0;

	pid_date->pid_in_p = 30;
	pid_date->pid_in_i = 0.8;
	pid_date->pid_in_d = 6;
	pid_date->pid_in_err = 0;
	pid_date->pid_in_err_max = 8;
	pid_date->pid_in_last_err = 0;
	pid_date->pid_in_integral_err = 0;
	pid_date->pid_in_integral_err_max = 300;
	pid_date->pid_in_control = 0;
}


  /**
  * @brief Cascade PID control function
  * @param[in] target_val: the target value to be achieved
  * @param[in] now_val: current position value
  * @param[in] pid_date: parameter structure
  */
void PID_Cascade(short target_val,short now_val,PID_CASCADE_DATE *pid_date)
{
	PID_Out(target_val, now_val, pid_date);
	PID_In(pid_date->pid_out_control, pid_date->pid_out_err-pid_date->pid_out_last_err, pid_date);
}

  /**
  * @brief Cascade outer loop PID control function
  * @param[in] target_val: the target value to be achieved
  * @param[in] now_val: current position value
  * @param[in] pid_date: parameter structure
  */
void PID_Out(short target_val,short now_val,PID_CASCADE_DATE *pid_date)
{
	pid_date->pid_out_last_err = pid_date->pid_out_err;
	pid_date->pid_out_err = target_val - now_val;
	
	if(pid_date->pid_out_err > pid_date->pid_out_err_max)
	{
		pid_date->pid_out_err = pid_date->pid_out_err_max;
	}
	pid_date->pid_out_integral_err += pid_date->pid_out_err;
	
	if(pid_date->pid_out_integral_err > pid_date->pid_out_integral_err_max)
	{
		pid_date->pid_out_integral_err = pid_date->pid_out_integral_err_max;
	}
	
	pid_date->pid_out_control = pid_date->pid_out_p*pid_date->pid_out_err +
	pid_date->pid_out_i*pid_date->pid_out_integral_err+
	pid_date->pid_out_d*(pid_date->pid_out_err-pid_date->pid_out_last_err);
}

  /**
  * @brief Cascade inner loop PID control function
  * @param[in] target_val: the target value to be achieved
  * @param[in] now_val: current position value
  * @param[in] pid_date: parameter structure
  */
void PID_In(short target_val,short now_val,PID_CASCADE_DATE *pid_date)
{
	pid_date->pid_in_last_err = pid_date->pid_in_err;
	pid_date->pid_in_err = target_val - now_val;
	pid_date->pid_in_integral_err += pid_date->pid_in_err;
	
	if(pid_date->pid_in_integral_err > pid_date->pid_in_integral_err_max)
	{
		pid_date->pid_in_integral_err = pid_date->pid_in_integral_err_max;
	}
	
	pid_date->pid_in_control = pid_date->pid_in_p*pid_date->pid_in_err +
	pid_date->pid_in_i*pid_date->pid_in_integral_err+
	pid_date->pid_in_d*(pid_date->pid_in_err-pid_date->pid_in_last_err);
}