/**
   *************************************************** ****************************
   * @file quaternion_fix.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.9.27
   * @note This program is ARM application programming (refer to the online routines, I add comments)
   *************************************************** ****************************
   */

#include "sysinc.h"



float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float exInt = 0, eyInt = 0, ezInt = 0;

float Yaw,Pitch,Roll;


  /**
  * @brief Quaternion update-three angles
  * @code
  * IMUupdate(acgy->gyr[0],acgy->gyr[1], acgy->gyr[2],acgy->acc[0], acgy->acc[1], acgy->acc[2] );
  * @endcode
  * @param[in] gx x-axis angular velocity, unit deg/s
  * @param[in] gy y-axis angular velocity, unit deg/s
  * @param[in] gz z-axis angular velocity, unit deg/s
  * @param[in] ax x-axis acceleration data, can be unitless
  * @param[in] ay y-axis acceleration data, can be unitless
  * @param[in] az z-axis acceleration data, can be unitless
  */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	norm = sqrt(ax*ax + ay*ay + az*az); //Normalized acceleration
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	vx = 2*(q1*q3 - q0*q2); //World coordinates obtained using angular velocity
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	ex = (ay*vz - az*vy); //The error between the coordinates obtained by angular velocity and the acceleration coordinates
	ey = (az*vx - ax*vz); //Tip: Three-dimensional coordinate operation, z*x=y
	ez = (ax*vy - ay*vx);

	exInt = exInt + ex*Ki; //Error integral
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	gx = gx + Kp*ex + exInt; //Corrected angular velocity
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT; //Update quaternion
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3); //Normalized quaternion
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; //Get the pitch angle and roll angle
	Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;
	//Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
}

