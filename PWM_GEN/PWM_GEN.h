#ifndef PWMPIN
#define PWMPIN 3

#ifndef PWMFREQ
#define PWMFREQ 100000

#ifndef PWM_NUM
#define PWM_NUM 256
#endif

void pwmsetup();

void pwmgen(float duty);