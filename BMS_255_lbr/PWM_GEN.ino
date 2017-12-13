#include "PWM_GEN.h"
#include <math.h>

int pwmsig;

void pwmsetup()
{
	analogWriteFrequency(PWMPIN,PWMFREQ);
}

void pwmgen(float duty)
{
	pwmsig = round(duty*PWM_NUM);
	analogWrite(PWMPIN,pwmsig);
}