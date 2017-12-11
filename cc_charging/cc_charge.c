/*!
  EE255 BMS constant current charging.
*/

#include "cc_charge.h"

float duty_new;
float cc_ch_error;
float cc_ch_integ = 0.0;

float CC_Charge(float curr)
{
	cc_ch_error = IDES_CH - curr;
	cc_ch_integ = cc_ch_integ + cc_ch_error * DT;
	duty_new = (cc_ch_integ * KI_CC_CH) + (cc_ch_error * KP_CC_CH);

	if (duty_new >= DMAX) {
		duty_new = DMAX;
	}
	else if (duty_new <= DMIN) {
		duty_new = DMIN;
	}

	return duty_new;
}