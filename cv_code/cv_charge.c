/*!
  EE255 BMS constant voltage charging.
*/

#include "cv_charge.h"

float cv_charge(float vol_pack)
{
	error_cv = V_REF - vol_pack
	integ = integ + error_cv * DT
	duty_new = (integ * KI_CV_CH) + (error_cv * KP_CV_CH)

	if (duty_new >= DMAX){
		duty_new = DMAX
	}
	else if (duty_new <= DMIN){
		duty_new = DMIN
	}

	return duty_new
}