/*!
  EE255 BMS constant voltage charging.
*/

#include "cv_charge.h"

float error_cv_ch;
float integ_cv_ch = 0.0;		
float duty_new;		

float cv_charge(float vol_pack)
{
	error_cv_ch = V_REF - vol_pack;
	integ_cv_ch = integ_cv_ch + error_cv_ch * DT;
	duty_new = (integ_cv_ch * KI_CV_CH) + (error_cv_ch * KP_CV_CH);

	if (duty_new >= DMAX){
		duty_new = DMAX;
	}
	else if (duty_new <= DMIN){
		duty_new = DMIN;
	}

	return duty_new;
}