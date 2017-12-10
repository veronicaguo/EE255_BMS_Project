#include "CC_Charge.h"

float duty_new
float cc_ch_error
float cc_ch_integ

float CC_Charge(float DUTY, float CURR)
{
	cc_ch_error = IDES-CURR
	cc_ch_integ = cc_ch_integ+cc_ch_error*DT
	duty_new = (cc_ch_integ*KI_CC_CH)+(cc_ch_error*KP_CC_CH)
	if duty_new >= DMAX
		duty_new = DMAX
	else if duty_new <= DMIN
		duty_new = DMIN
	return duty_new
}