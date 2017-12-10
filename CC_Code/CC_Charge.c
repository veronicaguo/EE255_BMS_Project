#include "CC_Charge.h"

float CC_Charge(float DUTY, float CURR)
{
	CC_CH_ERROR = IDES-CURR
	INTEG = INTEG+CC_CH_ERROR*DT
	DUTY_NEW = (INTEG*KI_CC_CH)+(ERROR*KP_CC_CH)
	if DUTY_NEW >= DMAX
		DUTY_NEW = DMAX
	else if DUTY_NEW <= DMIN
		DUTY_NEW = DMIN
	return DUTY_NEW
}